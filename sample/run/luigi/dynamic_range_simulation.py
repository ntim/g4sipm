#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class DynamicRangeSimulation(SimulationDynamicMetaTask, luigi.Task):
    name = luigi.Parameter("dynamic-range-simulation")
    n_repititions = luigi.IntParameter(10000)
    step = luigi.IntParameter(1000)
    
    n_min = luigi.IntParameter(1)  # minimum number of photons
    n_max = luigi.IntParameter(1000000)  # maximum number of photons
    log = luigi.BoolParameter(False)  # Dice number of photons in logarithmic bins.
    
    def run_kwargs(self):
        kwargs = dict(exe="../fast/fast", persist_hits=False, noise_if_no_signal=True)
        # Dice number of particles
        n = np.random.random_integers(self.n_min, self.n_max, self.step)
        if self.log:
            n = np.array(np.ceil(np.exp(np.uniform(np.log(self.n_min), np.log(self.n_max), self.step))), dtype="int")
        return [clone(kwargs, n_particles=ni) for ni in n]

    def run_after_yield(self):
        # Open results.
        inputs = self.sqlite_from_runs()
        with self.output().open("w") as o:
            for input in inputs:
                con = sqlite3.connect(input.fn)
                cur = con.cursor()
                try:
                    n_particles, t_min, t_max = cur.execute("SELECT nParticles, tMin, tMax FROM particleSourceMessenger;").fetchone()
                    n_eff_cells = np.sum(cur.execute("SELECT weight FROM `g4sipmDigis-0` WHERE time >= %s AND time < %s;" % (t_min, t_max)).fetchall())
                    print >> o, n_particles, n_eff_cells
                except Exception as e:
                    print "Failure in", input.fn
                    print e
    
class All(luigi.WrapperTask):
    def requires(self):
        models = []
        models.extend(glob.glob("../sample/resources/hamamatsu-s133*.properties"))
        models.extend(glob.glob("../sample/resources/sensl-*.properties"))
        return [DynamicRangeSimulation(model=model) for model in models]

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
