#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from ROOT import TEfficiency, TH1I

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in nanometers", default=10, type=int)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

# Prepare plot
f, ax = plt.subplots()

paths = glob.glob(args.path + "/*/g4sipm.tsv")
paths.sort()
for i, path in enumerate(paths):
    # Get SiPM properties
    con = sqlite3.connect(glob.glob(os.path.dirname(path) + "/*.sqlite")[0])
    cur = con.cursor()
    n_particles = cur.execute("SELECT nParticles FROM particleSourceMessenger;").fetchone()[0]
    name, pde, pct, papl, paps, n_cell, v_ov = cur.execute("SELECT name, pdeAt400nm, crosstalkProbability, apProbLong, apProbShort, numberOfCells, overVoltage FROM sipmModel;").fetchone()
    con.close()        
    # Read cached results from tsv file
    e, n = np.loadtxt(path, delimiter=" ").transpose()  # MeV, #
    # Determine bin width
    wvl = 4.135667516e-15 * 299792458.0 / e * 1e3
    wvl_min = min(wvl)
    wvl_max = max(wvl)
    nbins = int((wvl_max - wvl_min) / args.bin_width)
    eff = TEfficiency("%d-pass" % i, "%d-eff" % i, nbins, wvl_min, wvl_max)
    eff.SetUseWeightedEvents()
    eff.SetStatisticOption(TEfficiency.kFNormal)
    for wvli, ni in zip(wvl, n):
        eff.FillWeighted(True, ni, wvli)
        eff.FillWeighted(False, n_particles - ni, wvli)
    x = np.array([eff.GetTotalHistogram().GetBinCenter(i) for i in xrange(nbins)])
    y = np.array([eff.GetEfficiency(i) for i in xrange(nbins)]) * 100.0
    yerr_low = np.array([eff.ClopperPearson(int(n_particles), int(eff.GetEfficiency(i) * n_particles), 0.68, False) for i in xrange(nbins)]) * 100.0
    yerr_up = np.array([eff.ClopperPearson(int(n_particles), int(eff.GetEfficiency(i) * n_particles), 0.68, True) for i in xrange(nbins)]) * 100.0
    # Plot bin statistics.
    label = ("%s +%.1f V" % (name, v_ov * 1e6))
    # Plot PDE
    ax.errorbar(x, y, yerr=(y - yerr_low, yerr_up - y), fmt=".", label=label)

# Set legend titles.
ax.set_xlabel("wavelength / nm")
ax.set_ylabel("photon detection efficiency / %")

# Copyright
# ax.text(0.975, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="right", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)

# Create legends
ax.legend(loc="upper left")

# Tight layout
if args.tight_layout:
    f.tight_layout()

# Save plots
f.savefig("%s/pde-vs-wavelength.pdf" % args.path)

plt.show()
