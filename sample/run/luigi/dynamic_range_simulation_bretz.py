#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
import json
from simulation_meta_task import *

class DynamicRangeSimulation(SimulationDynamicMetaTask, luigi.Task):
    name = luigi.Parameter("dynamic-range-simulation-bretz")
    n_repititions = luigi.IntParameter(10000)
    step = luigi.IntParameter(5000)
    
    n_min = luigi.IntParameter(1)  # minimum number of photons
    n_max = luigi.IntParameter(1000000)  # maximum number of photons
    
    base_run_kwargs = luigi.Parameter("{}")
    
    def run_kwargs(self):
        kwargs = dict(exe="../fast/fast", persist_hits=False, noise_if_no_signal=True)
        kwargs.update(json.loads(self.base_run_kwargs))
        # print json.loads(self.base_run_kwargs)
        # Dice number of particles
        n = np.random.random_integers(self.n_min, self.n_max, self.step)
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
        model = "../sample/resources/hamamatsu-s13360-1325pe.properties"
        kwargs = [dict(temperature=10, bias_voltage=(52.19 + 5.00), path_spec="10-deg"),
                dict(temperature=25, bias_voltage=(53.00 + 5.00), path_spec="25-deg"),
                dict(temperature=40, bias_voltage=(53.81 + 5.00), path_spec="40-deg"),
                dict(temperature=25, bias_voltage=(53.00 + 5.00 - 0.054), path_spec="25-deg-0.054-V"),
                dict(temperature=25, bias_voltage=(53.00 + 5.00 + 0.054), path_spec="25-deg+0.054-V"),
                dict(temperature=25, bias_voltage=(53.00 + 5.00 - 0.005), path_spec="25-deg-0.005-V"),
                dict(temperature=25, bias_voltage=(53.00 + 5.00 + 0.005), path_spec="25-deg+0.005-V")
                ]
        return [DynamicRangeSimulation(model=model, path_spec=kw["path_spec"], base_run_kwargs=json.dumps(kw)) for kw in kwargs]

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
