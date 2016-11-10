#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class NPeSimulation(SimulationMetaTask, luigi.Task):
    name = luigi.Parameter("n-pe-simulation")
    n_repititions = luigi.IntParameter(1000)
    exe = luigi.Parameter("../fast/fast")  # Use the fast simulation.
    path_spec = luigi.Parameter()
    n_particles = luigi.IntParameter()
    
    @property
    def run_kwargs(self):
        d = dict(e_min=3.09960482523, e_max=3.09960482523, noise_if_no_signal=True, persist_hits=False, n_particles=self.n_particles)
        return [d] * self.n_repititions

    def run(self):
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
    
class AllModels(luigi.WrapperTask):
    n_particles = luigi.IntParameter()  # number of photons
    
    def requires(self):
        models = []
        # models.extend(glob.glob("../sample/resources/hamamatsu-s133*.properties"))
        # models.extend(glob.glob("../sample/resources/sensl-*.properties"))
        # models.extend(["../sample/resources/hamamatsu-s13360-6050pe.properties"])
        models.extend(glob.glob("../sample/resources/hamamatsu-s10362*.properties"))
        return [NPeSimulation(model=model, n_particles=self.n_particles, path_spec="/%d" % self.n_particles) for model in models]
    
class AllDynamic(luigi.Task):
    def run(self):
        n_particles = np.arange(1, 1000, 3)
        for n in n_particles:
            yield AllModels(n_particles=n)

if __name__ == "__main__":
    luigi.run(main_task_cls=AllDynamic)
