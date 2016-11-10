#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import luigi
import numpy as np
from config_meta_task import *

from subprocess import Popen, STDOUT
try:
    from subprocess import DEVNULL  # Since python 3
except ImportError:
    import os
    DEVNULL = open(os.devnull, "wb")
    
class Run(RunConfigMetaTask, luigi.Task):
    def run(self):
        # Create macro file.
        mac = self.output()[0]
        if not mac.exists():
            with mac.open("w") as o:
                # Particle source parameters
                print >> o, "/ps/nParticles", self.n_particles
                print >> o, "/ps/polar", self.polar, "deg"
                print >> o, "/ps/tMin 0 ns"
                print >> o, "/ps/tMax", self.pulse_width, "ns"
                if not is_none(self.t_input):
                    print >> o, "/ps/time/input", self.t_input
                print >> o, "/ps/energy/eMin", self.e_min, "eV"
                print >> o, "/ps/energy/eMax", self.e_max, "eV"
                if not is_none(self.e_input):
                    print >> o, "/ps/energy/input", self.e_input
                print >> o, "/ps/angle/phiMin", self.phi_min, "deg"
                print >> o, "/ps/angle/phiMax", self.phi_max, "deg"
                print >> o, "/ps/angle/thetaMin", self.theta_min, "deg"
                print >> o, "/ps/angle/thetaMax", self.theta_max, "deg"
                if not is_none(self.plane_surface_normal):
                    print >> o, "/ps/plane/surfaceNormal", self.plane_surface_normal, "mm"
                if not np.isnan(self.plane_a):
                    print >> o, "/ps/plane/a", self.plane_a, "mm"
                if not np.isnan(self.plane_b):
                    print >> o, "/ps/plane/b", self.plane_b, "mm"
                if not is_none(self.plane_pos):
                    print >> o, "/ps/plane/pos", self.plane_pos, "mm"
                # G4Sipm model parameters
                if self.seed >= 0:
                    print >> o, "/g4sipm/seed", self.seed
                print >> o, "/g4sipm/filter/geometry", self.filter_geometry
                print >> o, "/g4sipm/filter/pde", self.filter_pde
                print >> o, "/g4sipm/filter/timing", self.filter_timing
                print >> o, "/g4sipm/noise/thermal", self.noise_thermal
                if self.noise_pre_thermal >= 0:
                    print >> o, "/g4sipm/noise/preThermal", self.noise_pre_thermal
                if self.noise_post_thermal >= 0:
                    print >> o, "/g4sipm/noise/postThermal", self.noise_post_thermal
                print >> o, "/g4sipm/noise/afterpulse", self.noise_afterpulse
                print >> o, "/g4sipm/noise/crosstalk", self.noise_crosstalk
                if self.noise_crosstalk_neighbours >= 0:
                    print >> o, "/g4sipm/noise/crosstalkNeighbours", self.noise_crosstalk_neighbours
                print >> o, "/g4sipm/noise/ifNoSignal", self.noise_if_no_signal
                print >> o, "/g4sipm/digitize/hits", self.digitize_hits
                print >> o, "/g4sipm/digitize/trace", self.digitize_trace
                print >> o, "/g4sipm/digitize/shuntresistor", self.simulate_shuntresistor
                if not np.isnan(self.shuntresistor_recovery_time):
                    print >> o, "/g4sipm/digitize/shuntresistorRecoveryTime", self.shuntresistor_recovery_time
                # Persistency parameters
                if not np.isnan(self.persist_t_min):
                    print >> o, "/g4sipm/persist/tMin", self.persist_t_min, "ns"
                if not np.isnan(self.persist_t_max):
                    print >> o, "/g4sipm/persist/tMax", self.persist_t_max, "ns"
                print >> o, "/g4sipm/persist/hits", self.persist_hits
                print >> o, "/g4sipm/persist/digis", self.persist_digis
                # Tell the simulation to run
                print >> o, "/run/beamOn 1"
        # Execute simulation.
        output = self.output()[1]
        # Build command for invoking the simulation.
        exe = "ulimit -s 32768 && nice -n %d %s --model=%s --housing=%s --mac=%s --output=%s" % (self.nice_level, self.exe, self.model, self.housing, mac.fn, output.fn)
        if not np.isnan(self.bias_voltage):
            exe += " --bias-voltage=%.18e" % self.bias_voltage
        if not np.isnan(self.temperature):
            exe += " --temperature=%.18e" % self.temperature
        print exe
        # Run simulation in a shell, pipe output to /dev/null and wait for completion
        p = Popen(exe, stdout=DEVNULL, stderr=STDOUT, shell=True)
        p.communicate()
        if p.returncode != 0:
            # Simulation failed, so remove the output.
            if output.exists():
                output.remove()

    def output(self):
        if self.index < 0:
            ValueError("Index must not be below zero")
        return [luigi.LocalTarget("%s/g4sipm-%d.mac" % (self.path(), self.index)), luigi.LocalTarget("%s/g4sipm-%d.sqlite" % (self.path(), self.index))]
