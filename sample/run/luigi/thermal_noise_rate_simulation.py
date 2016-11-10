#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class ThermalNoiseRateSimulation(SimulationMetaTask, luigi.Task):
    name = luigi.Parameter("thermal-noise-rate-simulation")
    wavelength = luigi.FloatParameter(450)  # nm
    n_repititions = luigi.IntParameter(1000)
    bias_voltage_at_25_degrees = luigi.FloatParameter(58)
    progression_coefficient = luigi.FloatParameter(0.054)
    
    def bias_voltage(self, t):
        return self.bias_voltage_at_25_degrees + (t - 25.0) * self.progression_coefficient        
    
    @property
    def run_kwargs(self):
        if hasattr(self, '_cached_run_kwargs'):
            return self._cached_run_kwargs
        # Dice temperatures.
        temperatures = np.random.uniform(0, 40, self.n_repititions)
        # Set run_kwargs.
        e = 4.135667516e-15 * 299792458.0 / (self.wavelength * 1e-9)
        run_kwargs = dict(e_min=e, e_max=e, n_particles=1, noise_pre_thermal=3, noise_post_thermal=10, noise_if_no_signal=True, persist_t_min=0, persist_t_max=10e6, persist_hits=False, persist_digis=True)
        #
        self._cached_run_kwargs = [clone(run_kwargs, temperature=t, bias_voltage=self.bias_voltage(t)) for t in temperatures]
        return self._cached_run_kwargs
    
    def run(self):
        inputs = self.sqlite_from_runs()
        with self.output().open("w") as o:
            for input in inputs:
                con = sqlite3.connect(input.fn)
                cur = con.cursor()
                try:
                    # TODO get angle.
                    temp, vb, vov, f_th = cur.execute("SELECT temperature, breakdownVoltage, overVoltage, thermalNoiseRate FROM sipmModel;").fetchone()
                    n = np.sum(cur.execute("SELECT weight FROM `g4sipmDigis-0` WHERE type = 2;").fetchall())
                    print >> o, temp, vb, vov, f_th, n
                except Exception as e:
                    print "Failure in", input.fn
                    print e
    
class All(luigi.WrapperTask):
    def requires(self):
        models = []
        models.extend(glob.glob("../sample/resources/hamamatsu-s13360-60*.properties"))
        return [ThermalNoiseRateSimulation(model=model) for model in models]

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
