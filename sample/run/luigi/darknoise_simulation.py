#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class DarknoiseConfigMetaTask(ConfigMetaTask):
    name = luigi.Parameter("darknoise-simulation")
    # "Signal" silence required to identify thermal noise pulses
    silence = luigi.FloatParameter(default=300)  # ns

class DarknoiseSimulation(DarknoiseConfigMetaTask, SimulationMetaTask, luigi.Task):
    n_repititions = luigi.IntParameter(1000)
    
    @property
    def run_kwargs(self):
        return [dict(n_particles=1, noise_pre_thermal=3, noise_post_thermal=10, noise_if_no_signal=True, persist_t_min=0, persist_t_max=10e6, persist_hits=False, persist_digis=True)] * self.n_repititions

    def run(self):
        inputs = self.sqlite_from_runs()
        with self.output().open("w") as o:
            for input in inputs:
                con = sqlite3.connect(input.fn)
                cur = con.cursor()
                try:
                    # Get persisted digis.
                    digis = np.array(cur.execute("SELECT time, weight FROM `g4sipmDigis-0`;").fetchall())
                    time = digis[:, 0]
                    weight = digis[:, 1]
                    # Histogram.
                    bins = np.arange(min(time), max(time) + 1)
                    hist, bin_edges = np.histogram(time, bins=bins, weights=weight)
                    for h, bin in zip(hist, bin_edges[:-1]):
                        if h > 0.0:
                            print >> o, bin, h
                except Exception as e:
                    print "Failure in", input.fn
                    raise e

class DeltaTDistribution(DarknoiseConfigMetaTask, luigi.Task):
    def requires(self):
        return self.clone(DarknoiseSimulation)
    
    def run(self):
        with self.output().open("w") as o:
            data = np.loadtxt(self.input().fn, delimiter=" ")
            time = data[:, 0]
            pe = data[:, 1]
            i = 1
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
                        print >> o, diff_next, pe[i + 1]
                        # Skipt this pulse for next iteration
                        i += 1
                i += 1

    def output(self):
        return luigi.LocalTarget("%s/g4sipm-delta-t.tsv" % (self.path()))

class PeDistribution(DarknoiseConfigMetaTask, luigi.Task):
    def requires(self):
        return self.clone(DarknoiseSimulation)
    
    def run(self):
        with self.output().open("w") as o:
            data = np.loadtxt(self.input().fn, delimiter=" ")
            time = data[:, 0]
            pe = data[:, 1]
            i = 1
            while i < len(time):
                # Select a thermal noise induced pulse
                diff_prev = time[i] - time[i - 1]
                if diff_prev > self.silence:
                        # ... the pulse is thermally induced.
                        # We measure the time and pulse height to the next pulse
                        print >> o, pe[i]
                        # Skipt this pulse for next iteration
                        i += 1
                i += 1

    def output(self):
        return luigi.LocalTarget("%s/g4sipm-pe.tsv" % (self.path()))

class All(luigi.WrapperTask):
    def requires(self):
        models = []
        #models.extend(glob.glob("../sample/resources/hamamatsu-*.properties"))
        #models.extend(glob.glob("../sample/resources/sensl-*.properties"))
        models.append("HamamatsuS12573100X")
        return [[DeltaTDistribution(model=model), PeDistribution(model=model)] for model in models]

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
