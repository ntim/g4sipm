#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class PdeSimulation(SimulationMetaTask, luigi.Task):
    name = luigi.Parameter("pde-simulation")
    n_repititions = luigi.IntParameter(1000)
    
    @property
    def run_kwargs(self):
        if hasattr(self, '_cached_run_kwargs'):
            return self._cached_run_kwargs
        # Dice energies.
        wvl_min = 4.135667516e-15 * 299792458.0 / Run(name=self.name).e_max
        wvl_max = 4.135667516e-15 * 299792458.0 / Run(name=self.name).e_min
        wvl = np.random.uniform(wvl_min, wvl_max, self.n_repititions)
        e = 4.135667516e-15 * 299792458.0 / wvl
        # Set run_kwargs.
        run_kwargs = dict(n_particles=10000, digitize_hits=False, persist_hits=True, persist_digis=False)
        #
        self._cached_run_kwargs = [clone(run_kwargs, e_min=ei, e_max=ei) for ei in e]
        return self._cached_run_kwargs
    
    def run(self):
        inputs = self.sqlite_from_runs()
        with self.output().open("w") as o:
            for input in inputs:
                con = sqlite3.connect(input.fn)
                cur = con.cursor()
                try:
                    # TODO get angle.
                    e = cur.execute("SELECT eMin FROM particleSourceMessenger;").fetchone()[0]
                    n = cur.execute("SELECT count() FROM `g4sipmHits-0`;").fetchone()[0]
                    print >> o, e, n
                except Exception as e:
                    print "Failure in", input.fn
                    print e
    
class All(luigi.WrapperTask):
    def requires(self):
        models = []
        models.extend(glob.glob("../sample/resources/hamamatsu-*.properties"))
        models.extend(glob.glob("../sample/resources/sensl-*.properties"))
        return [PdeSimulation(model=model) for model in models]

if __name__ == "__main__":
    luigi.run()
