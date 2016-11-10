#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import luigi
import numpy as np
from types import NoneType
import ast

def is_none(param):
    """
    Checks if param is None or "None".
    """
    if param is None:
        return True
    if type(param) == str:
        try:
            if ast.literal_eval(param) is None:
                return True
        except:
            pass
    return False

def basename(path):
    """
    Returns the base name of the path.
    """
    return os.path.splitext(os.path.basename(path))[0]

def clone(dictionary, **kwargs):
    """
    Clones the dictionary and overrides the supplied parameters.
    """
    d = dict(dictionary)
    for k in kwargs:
        d[k] = kwargs[k]
    return d

class ConfigMetaTask:
    """
    Configures the parameters important for the task signature.
    """
    index = luigi.IntParameter(default=-1)
    name = luigi.Parameter()
    model = luigi.Parameter(default="../sample/resources/sensl-microfc-60035-sm.properties")
    path_spec = luigi.Parameter(default=None)
    exe = luigi.Parameter(default="./sample")

    def path(self):
        model = self.model
        # Is a properties file.
        if os.path.exists(model):
            model = basename(model)
        p = "./results/%s/%s" % (self.name, model)
        if not is_none(self.path_spec):
            if self.path_spec.startswith("/"):
                p += self.path_spec
            else:
                p += "-" + self.path_spec
        return p

class RunConfigMetaTask(ConfigMetaTask):
    """
    Configures the specific settings for a single run used for generating the mac file. 
    The following parameters are all considered not significant.
    """

    # Call signature parameters
    housing = luigi.Parameter(default="default", significant=False)
    temperature = luigi.FloatParameter(default=np.nan, significant=False)  # celsius
    bias_voltage = luigi.FloatParameter(default=np.nan, significant=False)  # volts
    nice_level = luigi.IntParameter(default=0, significant=False)

    # Particle source parameters
    n_particles = luigi.IntParameter(default=1000, significant=False)
    polar = luigi.FloatParameter(default=-360, significant=False)  # deg
    pulse_width = luigi.FloatParameter(default=25, significant=False)  # ns
    t_input = luigi.Parameter(default=None, significant=False)
    e_min = luigi.FloatParameter(default=1.13, significant=False)  # eV (~1100 nm)
    e_max = luigi.FloatParameter(default=6.2, significant=False)  # eV (~200 nm)
    e_input = luigi.Parameter(default=None, significant=False)
    phi_min = luigi.FloatParameter(default=0, significant=False)  # deg
    phi_max = luigi.FloatParameter(default=0, significant=False)  # deg
    theta_min = luigi.FloatParameter(default=0, significant=False)  # deg
    theta_max = luigi.FloatParameter(default=0, significant=False)  # deg
    plane_surface_normal = luigi.Parameter(default=None, significant=False)  # Format: "1 1 1" in mm
    plane_a = luigi.FloatParameter(default=np.nan, significant=False)  # mm
    plane_b = luigi.FloatParameter(default=np.nan, significant=False)  # mm
    plane_pos = luigi.Parameter(default=None, significant=False)  # Format: "1 1 1" in mm

    # G4Sipm model parameters
    seed = luigi.IntParameter(default=-1, significant=False)
    filter_geometry = luigi.BoolParameter(default=True, significant=False)
    filter_pde = luigi.BoolParameter(default=True, significant=False)
    filter_timing = luigi.BoolParameter(default=True, significant=False)
    noise_thermal = luigi.BoolParameter(default=True, significant=False)
    noise_pre_thermal = luigi.IntParameter(default=-1, significant=False)
    noise_post_thermal = luigi.IntParameter(default=-1, significant=False)
    noise_afterpulse = luigi.BoolParameter(default=True, significant=False)
    noise_crosstalk = luigi.BoolParameter(default=True, significant=False)
    noise_crosstalk_neighbours = luigi.IntParameter(default=-1, significant=False)  # 4, 8, 24, 48, 80, 120 ...
    noise_if_no_signal = luigi.BoolParameter(default=False, significant=False)
    digitize_hits = luigi.BoolParameter(default=True, significant=False)
    digitize_trace = luigi.BoolParameter(default=False, significant=False)
    simulate_shuntresistor = luigi.BoolParameter(default=False, significant=False)
    shuntresistor_recovery_time = luigi.FloatParameter(default=np.nan, significant=False)

    # Persistency parameters
    persist_t_min = luigi.FloatParameter(default=np.nan, significant=False)  # ns
    persist_t_max = luigi.FloatParameter(default=np.nan, significant=False)  # ns
    persist_hits = luigi.BoolParameter(default=True, significant=False)
    persist_digis = luigi.BoolParameter(default=True, significant=False)
