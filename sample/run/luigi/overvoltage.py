#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class OvervoltageSimulation(SimulationMetaTask, luigi.Task):
    name = luigi.Parameter("overvoltage-simulation")
    wavelength = luigi.FloatParameter(450)  # nm
    n_repititions = luigi.IntParameter(1000) 
    breakdown_voltage = luigi.FloatParameter(53) # Volt
    
    @property
    def run_kwargs(self):
        if hasattr(self, '_cached_run_kwargs'):
            return self._cached_run_kwargs
        # Dice temperatures.
        overvoltages = np.random.uniform(0, 10, self.n_repititions)
        # Set run_kwargs.
        e = 4.135667516e-15 * 299792458.0 / (self.wavelength * 1e-9)
        run_kwargs = dict(e_min=e, e_max=e, n_particles=10000, digitize_hits=False, persist_hits=True, persist_digis=False)
        #
        self._cached_run_kwargs = [clone(run_kwargs, bias_voltage=(self.breakdown_voltage + ov)) for ov in overvoltages]
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
                    n = cur.execute("SELECT count() FROM `g4sipmHits-0`;").fetchone()[0]
                    print >> o, temp, vb, vov, f_th, n
                except Exception as e:
                    print "Failure in", input.fn
                    print e
    
class All(luigi.WrapperTask):
    def requires(self):
        models = []
        models.extend(glob.glob("../sample/resources/hamamatsu-s13360-60*.properties"))
        return [OvervoltageSimulation(model=model) for model in models]

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
