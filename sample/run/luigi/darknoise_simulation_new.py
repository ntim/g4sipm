#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import luigi.contrib.external_program
import sqlite3
import pickle
import os
import numpy as np
import pandas as pd
import uncertainties as u
from contrib import afterpulse, darknoise, crosstalk

def is_none(param):
    """
    Checks if param is None or "None".
    """
    if param is None:
        return True
    if type(param) == str:
        try:
            if ast.literal_eval(param) is None:
                return True
        except:
            pass
    return False

def basename(path):
    """
    Returns the base name of the path.
    """
    return os.path.splitext(os.path.basename(path))[0]

class RunMetaTask():
    id = luigi.IntParameter(-1)
    name = luigi.Parameter()
    model = luigi.Parameter()
    bias_voltage = luigi.FloatParameter(default=np.nan)
    path_spec = luigi.Parameter(default=None)
    silence = luigi.IntParameter(400)

    def path(self):
        model = self.model
        # Is a properties file.
        if os.path.exists(model):
            model = basename(model)
        p = "./results/%s/%s" % (self.name, model)
        if not is_none(self.path_spec):
            if self.path_spec.startswith("/"):
                p += self.path_spec
            else:
                p += "-" + self.path_spec
        return p


class Run(luigi.contrib.external_program.ExternalProgramTask, RunMetaTask):
    nice_level = luigi.IntParameter(5)

    def program_args(self):
        # Create macro file.
        mac = self.output()[0]
        if not mac.exists():
            with mac.open("w") as o:
                # Particle source parameters
                print >> o, "/ps/nParticles", 1
                print >> o, "/g4sipm/noise/preThermal", 3
                print >> o, "/g4sipm/noise/postThermal", 10
                print >> o, "/g4sipm/noise/crosstalkNeighbours", 4
                print >> o, "/g4sipm/noise/ifNoSignal", True
                print >> o, "/g4sipm/digitize/hits", True
                print >> o, "/g4sipm/digitize/trace", False
                # Persistency parameters
                print >> o, "/g4sipm/persist/tMin", 0, "ns"
                print >> o, "/g4sipm/persist/tMax", 10e6, "ns"
                print >> o, "/g4sipm/persist/hits", False
                print >> o, "/g4sipm/persist/digis", True
                # Tell the simulation to run
                print >> o, "/run/beamOn 1"
       # Execute simulation.
        output = self.output()[1]
        args = [  # "ulimit", "-s", "32768", "&&", 
                "nice", "-n", self.nice_level,
                "./sample", "--mac", mac.fn, "--output", output.fn, "--model", self.model]
        if not np.isnan(self.bias_voltage):
            args.extend(["--bias-voltage", self.bias_voltage])
        return args
        
    def output(self):
        return [luigi.LocalTarget("%s/g4sipm-%d.mac" % (self.path(), self.id)), luigi.LocalTarget("%s/g4sipm-%d.sqlite" % (self.path(), self.id)) ]


class ExportRunSettings(luigi.Task, RunMetaTask):
    
    def requires(self):
        return self.clone(Run, index=0)

    def run(self):
        tables = ['particleSourceMessenger', 'g4sipmUiMessenger', 'sipmModel', 'sipmVoltageTraceModel']
        if self.input()[1].exists():
            input = self.input()[1].fn
            # Open database.
            con = sqlite3.connect(input)
            con.row_factory = sqlite3.Row
            cur = con.cursor()
            # Select first entry of all tables.
            settings = dict()
            for table in tables:
                row = cur.execute("SELECT * FROM `%s`;" % table).fetchone()
                # Iterate all columns.
                entry = dict()
                for key, value in zip(row.keys(), row):
                    entry[key] = value
                # Add to settings dictionary
                settings[table] = entry
            # Pickle settings
            with self.output().open("w") as o:
                pickle.dump(settings, o)

    def output(self):
        return luigi.LocalTarget("%s/g4sipm.pkl" % (self.path()))


class Histogram(luigi.Task, RunMetaTask):
    
    def requires(self):
        return self.clone(Run)
    
    def run(self):
        con = sqlite3.connect(self.input()[1].fn)
        cur = con.cursor()
        try:
            # Get persisted digis.
            digis = np.array(cur.execute("SELECT time, weight FROM `g4sipmDigis-0`;").fetchall())
            time = digis[:, 0]
            weight = digis[:, 1]
            # Histogram.
            bins = np.arange(np.floor(0), np.ceil(time.max() - 5000) + 1)
            hist, bin_edges = np.histogram(time, bins=bins, weights=weight)
            # Create dataframe.
            df = pd.DataFrame({"time": bin_edges[:-1], "pe": hist})
            # Drop entries with zero.
            df = df[df.pe != 0]
            # Save
            df.to_pickle(self.output().fn)
        except Exception as e:
            print ("Failure in", self.input()[1].fn)
            raise e
                
    def output(self):
        return luigi.LocalTarget("%s/g4sipm-%d-hist.pkl" % (self.path(), self.id))


class DeltaTDistribution(luigi.Task, RunMetaTask):
    
    def requires(self):
        return self.clone(Histogram)
    
    def run(self):
        df = pd.read_pickle(self.input().fn)
        time = df.time.values
        pe = df.pe.values
        i = 1
        time_diff, next_pe = [], []
        while i < len(time) - 1:
            # Select a thermal noise pulse
            # If the pulse has 1 p.e. ...
            if pe[i] > 0.5 and pe[i] < 1.5:
                diff_prev = time[i] - time[i - 1]
                diff_next = time[i + 1] - time[i]
                # ... and the time difference to the previous pulse is large enough ...
                if diff_prev > self.silence and diff_next > 0.0:
                    # ... the pulse is thermally induced.
                    # We measure the time and pulse height to the next pulse
                    time_diff.append(diff_next)
                    next_pe.append(pe[i + 1])
                    # Skipt this pulse for next iteration
                    i += 1
            i += 1
        df = pd.DataFrame({"time_diff": time_diff, "pe": next_pe})
        df.to_pickle(self.output().fn)
                
    def output(self):
        return luigi.LocalTarget("%s/g4sipm-%d-delta-t.pkl" % (self.path(), self.id))
        
    
class PeDistribution(luigi.Task, RunMetaTask):
    
    def requires(self):
        return self.clone(Histogram)
    
    def run(self):
        df = pd.read_pickle(self.input().fn)
        time = df.time.values
        pe = df.pe.values
        i = 1
        curr_pe = []
        while i < len(time):
            # Select a thermal noise induced pulse
            diff_prev = time[i] - time[i - 1]
            if diff_prev > self.silence and pe[i] > 0.0:
                # ... the pulse is thermally induced.
                # We measure the time and pulse height to the next pulse
                curr_pe.append(pe[i])
                # Skipt this pulse for next iteration
                i += 1
            i += 1
        df = pd.DataFrame({"pe": curr_pe})
        df.to_pickle(self.output().fn)
                
    def output(self):
        return luigi.LocalTarget("%s/g4sipm-%d-pe.pkl" % (self.path(), self.id))
    

class DarknoiseDeltaTSimulation(luigi.Task, RunMetaTask):
    n_repititions = luigi.IntParameter(1000)
    
    def requires(self):
        return [self.clone(DeltaTDistribution, id=i) for i in xrange(self.n_repititions)]
    
    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("%s/g4sipm-delta-t.pkl" % (self.path()))
    

class DarknoisePeSimulation(luigi.Task, RunMetaTask):
    n_repititions = luigi.IntParameter(1000)
    
    def requires(self):
        return [self.clone(PeDistribution, id=i) for i in xrange(self.n_repititions)]
    
    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("%s/g4sipm-pe.pkl" % (self.path()))
    

class DeltaTFit(luigi.Task, RunMetaTask):
    bin_width = luigi.IntParameter(10)
    t_max = luigi.IntParameter(5000)
    
    def requires(self):
        return [self.clone(DarknoiseDeltaTSimulation), self.clone(ExportRunSettings)]
    
    def run(self):
        df = pd.read_pickle(self.input()[0].fn)
        g4sipm = pickle.load(self.input()[1].open("r"))
        # Create histogram
        from ROOT import TH1D, TFile
        nbins = self.t_max / self.bin_width
        f = TFile(self.output()[0].fn, "RECREATE")
        h = TH1D("hist", ";time/ns;entries", nbins , 0, self.t_max)
        h.Sumw2()
        for ti in df.time_diff:
            h.Fill(ti)
        # Normalize
        h.Scale(1.0 / h.Integral("width"))
        # Perform fit.
        amp, amp_err, p_ap_s, p_ap_s_err, p_ap_l, p_ap_l_err, tau_s, tau_s_err, tau_l, tau_l_err, tau_th, tau_th_err, chi2, ndof = afterpulse.fit(h, xlow=50)
        #
        h.Write()
        f.Close()
        # Create dataframe with results and save.
        df = pd.DataFrame({"amp_delta_t": [u.ufloat(amp, amp_err)],
                           "p_ap_f": [u.ufloat(p_ap_s, p_ap_s_err)],
                           "p_ap_s": [u.ufloat(p_ap_l, p_ap_l_err)],
                           "tau_ap_f": [u.ufloat(tau_s, tau_s_err)],
                           "tau_ap_s": [u.ufloat(tau_l, tau_l_err)],
                           "tau_th": [u.ufloat(tau_th, tau_th_err)],
                           "chi2_delta_t": [chi2],
                           "ndof_delta_t": [ndof],
                           })
        df.to_pickle(self.output()[1].fn)
        
    def output(self):
        return [luigi.LocalTarget("%s/g4sipm-delta-t-fit.root" % (self.path())), luigi.LocalTarget("%s/g4sipm-delta-t-fit.pkl" % (self.path()))]


class PeFit(luigi.Task, RunMetaTask):
    bin_width = luigi.FloatParameter(0.1)
    pe_max = luigi.IntParameter(12)
    
    def requires(self):
        return [self.clone(DarknoisePeSimulation), self.clone(ExportRunSettings)]
    
    def run(self):
        df = pd.read_pickle(self.input()[0].fn)
        g4sipm = pickle.load(self.input()[1].open("r"))
        ncell = g4sipm["sipmModel"]["numberOfCells"]
        neighbours = g4sipm["g4sipmUiMessenger"]["noiseCrosstalkNeighbours"]
        # Create histogram
        from ROOT import TH1D, TFile, TF1, ROOT
        nbins = int(self.pe_max / self.bin_width)
        # Create and fill histogram.
        f = TFile(self.output()[0].fn, "RECREATE")
        h = TH1D("hist", "hist", nbins , 0 + self.bin_width / 2.0, self.pe_max + self.bin_width / 2.0)
        h.Sumw2()
        for p in df.pe:
            h.Fill(p)
        # Perform fit
        p_ct_fit, p_ct_fit_err, amp, amp_err, g, g_err, sigma_pe, sigma_pe_err, p_ct, p_ct_err, mu_0, mu_0_err, sigma_el, sigma_el_err, nu, nu_err, chi2, ndof = darknoise.fit(h, ncell, neighbours)
        #
        h.Write()
        f.Close()
        # Create dataframe with results and save.
        df = pd.DataFrame({"p_ct_fit": [u.ufloat(p_ct_fit, p_ct_fit_err)],
                           "amp_pe": [u.ufloat(amp, amp_err)],
                           "g": [u.ufloat(g, g_err)],
                           "sigma_pe": [u.ufloat(sigma_pe, sigma_pe_err)],
                           "p_ct": [u.ufloat(p_ct, p_ct_err)],
                           "mu_0": [u.ufloat(mu_0, mu_0_err)],
                           "sigma_el": [u.ufloat(sigma_el, sigma_el_err)],
                           "nu": [u.ufloat(nu, nu_err)],
                           "chi2_pe": [chi2],
                           "ndof_pe": [ndof],
                           })
        df.to_pickle(self.output()[1].fn)
        
    def output(self):
        return [luigi.LocalTarget("%s/g4sipm-pe-fit.root" % (self.path())), luigi.LocalTarget("%s/g4sipm-pe-fit.pkl" % (self.path()))]


class DarknoiseSimulation(luigi.Task, RunMetaTask):
    name = luigi.Parameter("darknoise-simulation")
    
    def requires(self):
        yield self.clone(DeltaTFit)
        yield self.clone(PeFit)
        
    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input[1].fn))
        df = pd.concat(frames, axis=1, join="inner")
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("%s/darknoise-simulation.pkl" % (self.path()))        
    
    
class AllOvervoltagesForHamamatsuS1036233050C(luigi.Task, RunMetaTask):
    name = luigi.Parameter("darknoise-simulation")
    model = luigi.Parameter("./resources/hamamatsu-s10362-33-050c-niggemann.properties")
    
    def requires(self):
        v_break = 70.0
        v_ov = [0.71, 0.81, 0.91, 1.01, 1.11, 1.21, 1.31, 1.41]
        return [DarknoiseSimulation(name=self.name, model=self.model, path_spec=str(v), bias_voltage=(v_break + v)) for v in v_ov]

    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df = pd.concat([df, pd.DataFrame({"v_ov": [0.71, 0.81, 0.91, 1.01, 1.11, 1.21, 1.31, 1.41]})], axis=1, join="inner")
        self.output()[0].makedirs()
        df.to_pickle(self.output()[0].fn)
        # Get expectation from model files.
        v_ov, f_th, p_ct, p_ap_s, p_ap_f, tau_ap_s, tau_ap_f = [], [], [], [], [], [], []
        for input in self.input():
            g4sipm = pickle.load(open(os.path.join(os.path.dirname(input.fn), "g4sipm.pkl")))
            v_ov.append(g4sipm["sipmModel"]["overVoltage"] * 1e6)  # volt
            f_th.append(g4sipm["sipmModel"]["thermalNoiseRate"]) 
            p_ct.append(g4sipm["sipmModel"]["crossTalkProbability"])
            p_ap_s.append(g4sipm["sipmModel"]["apProbLong"])
            p_ap_f.append(g4sipm["sipmModel"]["apProbShort"])
            tau_ap_s.append(g4sipm["sipmModel"]["apTauLong"])
            tau_ap_f.append(g4sipm["sipmModel"]["apTauShort"])
        df = pd.DataFrame({"v_ov":v_ov, "f_th":f_th, "p_ct":p_ct, "p_ap_s":p_ap_s, "p_ap_f":p_ap_f, "tau_ap_s":tau_ap_s, "tau_ap_f":tau_ap_f})
        df.to_pickle(self.output()[1].fn)
        
    def output(self):
        return [luigi.LocalTarget("%s/darknoise-simulation.pkl" % (self.path())), luigi.LocalTarget("%s/darknoise-simulation-expected.pkl" % (self.path()))]


if __name__ == "__main__":
    luigi.run()
    
