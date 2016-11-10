#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob, pickle
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from analysis import crosstalk, darknoise
from contrib import legend, natsort
from ROOT import TEfficiency

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in volt", default=0.25, type=float)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

paths = natsort.sort(glob.glob(args.path + "/*/g4sipm.tsv"))
for i, path in enumerate(paths):
    # Prepare plots
    f1, ax1 = plt.subplots() 
    f2, ax2 = plt.subplots() 
    
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
    
    # Bin PDE
    vov_min = min(vov)
    vov_max = max(vov)
    nbins = int((vov_max - vov_min) / args.bin_width)
    eff = TEfficiency("%d-pass" % i, "%d-eff" % i, nbins, vov_min, vov_max)
    eff.SetUseWeightedEvents()
    eff.SetStatisticOption(TEfficiency.kFNormal)
    for vovi, ni in zip(vov, n):
        eff.FillWeighted(True, ni, vovi)
        eff.FillWeighted(False, n_particles - ni, vovi)
    x = np.array([eff.GetTotalHistogram().GetBinCenter(i) for i in xrange(nbins)])
    y = np.array([eff.GetEfficiency(i) for i in xrange(nbins)]) * 100.0
    yerr_low = np.array([eff.ClopperPearson(int(n_particles), int(eff.GetEfficiency(i) * n_particles), 0.68, False) for i in xrange(nbins)]) * 100.0
    yerr_up = np.array([eff.ClopperPearson(int(n_particles), int(eff.GetEfficiency(i) * n_particles), 0.68, True) for i in xrange(nbins)]) * 100.0
    # Plot PDE
    ax1.errorbar(x, y, yerr=(y - yerr_low, yerr_up - y), fmt=".", label=name)
    ax1.set_xlabel("overvoltage / V")
    ax1.set_ylabel("photon detection efficiency / %")
    
    ax2.plot(vov, f_th, '.')
    ax2.set_xlabel("overvoltage / V")
    ax2.set_ylabel("thermal noise rate / Hz")
    
plt.show()