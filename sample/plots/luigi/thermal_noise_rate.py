#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob, pickle
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from analysis import crosstalk, darknoise
from contrib import legend, natsort, histogram
from ROOT import TEfficiency

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in Celsius", default=1, type=float)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

paths = natsort.sort(glob.glob(args.path + "/*/g4sipm.tsv"))
for i, path in enumerate(paths):
    # Prepare plots
    f1, ax1 = plt.subplots() 
    
    # Get SiPM properties
    g4sipm = pickle.load(open(os.path.dirname(path) + "/g4sipm.pkl"))
    n_particles = g4sipm["particleSourceMessenger"]["nParticles"]
    name = g4sipm["sipmModel"]["name"]

    # Read cached results from tsv file
    temp, vb, vov, f_th, n = np.loadtxt(path, delimiter=" ").transpose()
    # To volts.
    vb *= 1e6
    vov *= 1e6
    # To kHz
    f_th *= 1e9 / 1e3
    # To Celsius
    temp -= 273.15
    
    # Bin PDE
    xlow = min(temp)
    xup = max(temp)
    nbins = (xup - xlow) / args.bin_width
    x, y, yerr, _ = histogram.profile(temp, n, "fth", nbins, xlow, xup)
    # Plot PDE
    ax1.errorbar(x, y, yerr=yerr, fmt=".", label=name)
    ax1.set_xlabel("temperature / Celsius")
    ax1.legend()
    
plt.show()
