#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
import json
from simulation_meta_task import *

class DynamicRangeSimulation(SimulationDynamicMetaTask, luigi.Task):
    name = luigi.Parameter("dynamic-range-simulation-schumacher")
    n_repititions = luigi.IntParameter(1000)
    step = luigi.IntParameter(1000)
    
    n_min = luigi.IntParameter(1)  # minimum number of photons
    n_max = luigi.IntParameter(1e7)  # maximum number of photons
    t_input = luigi.Parameter("../sample/resources/sawtooth-100ps.properties")
    e_min = luigi.FloatParameter(default=3.061338207066896, significant=False)  # eV (405 nm)
    e_max = luigi.FloatParameter(default=3.061338207066896, significant=False)  # eV (405 nm)
    
    def run_kwargs(self):
        kwargs = dict(exe="../fast/fast", persist_hits=False, noise_if_no_signal=True, t_input=self.t_input, bias_voltage=56.7)
        # Dice number of particles
        n = np.array(np.ceil(np.exp(np.random.uniform(np.log(self.n_min), np.log(self.n_max), self.step))), dtype="int")
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
        return DynamicRangeSimulation(model="../sample/resources/hamamatsu-s13360-6050pe.properties")

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
