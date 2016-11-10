#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from simulation_meta_task import *

class RelativePdeSimulation(SimulationMetaTask, luigi.Task):
    name = luigi.Parameter("relative-pde-simulation")
    wavelength = luigi.FloatParameter(450)  # nm
    # TODO the plane distance and size must cover the largest sensor simulated
    source_distance = luigi.FloatParameter(8)  # mm
    n_repititions = luigi.IntParameter(1000)
    
    @property
    def run_kwargs(self):
        if hasattr(self, '_cached_run_kwargs'):
            return self._cached_run_kwargs
        # Calculate energy.
        # Randomize angles.
        theta = np.random.uniform(0, np.pi / 2.0, self.n_repititions)
        # Calculate source position
        x, y, z = self.source_distance * np.sin(theta), np.zeros(self.n_repititions), self.source_distance * np.cos(theta)
        plane_pos = ["%.18e %.18e %.18e" % (xi, yi, zi) for xi, yi, zi in zip(x, y, z)]
        # Calculate source plane surface normal.
        nx, ny, nz = -np.sin(theta), np.zeros(self.n_repititions), -np.cos(theta)
        plane_surface_normal = ["%.18e %.18e %.18e" % (nxi, nyi, nzi) for nxi, nyi, nzi in zip(nx, ny, nz)]
        # Set run_kwargs.
        e = 4.135667516e-15 * 299792458.0 / (self.wavelength * 1e-9)
        run_kwargs = dict(n_particles=30000, housing="ceramic", plane_a=8, plane_b=8, digitize_hits=False, persist_hits=True, persist_digis=False, e_min=e, e_max=e)
        #
        self._cached_run_kwargs = [clone(run_kwargs, plane_pos=plane_pos[i], plane_surface_normal=plane_surface_normal[i]) for i in xrange(self.n_repititions)]
        return self._cached_run_kwargs
 
    def run(self):
        inputs = self.sqlite_from_runs()
        with self.output().open("w") as o:
            for input in inputs:
                con = sqlite3.connect(input.fn)
                cur = con.cursor()
                try:
                    # Get angle.
                    surface_normal = cur.execute("SELECT surfaceNormal FROM particleSourceMessenger;").fetchone()[0]
                    z = float(surface_normal[1:-1].split(',')[2])
                    theta = np.pi - np.arccos(z)
                    n = cur.execute("SELECT count() FROM `g4sipmHits-0`;").fetchone()[0]
                    print >> o, theta, n / np.cos(theta)
                except Exception as e:
                    print "Failure in", input.fn
                    print e
    
class All(luigi.WrapperTask):
    def requires(self):
        models = []
        models.extend(glob.glob("../sample/resources/hamamatsu-*.properties"))
        models.extend(glob.glob("../sample/resources/sensl-*.properties"))
        return [RelativePdeSimulation(model=model) for model in models]

if __name__ == "__main__":
    luigi.run()
