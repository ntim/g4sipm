#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import pickle
import os, sys, glob
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from contrib import histogram
from analysis import fresnel

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in degrees", default=2, type=int)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

# Prepare plot
f1, ax1 = plt.subplots()
f2, ax2 = plt.subplots()

paths = glob.glob(args.path + "/*/g4sipm.tsv")
paths.sort()
for i, path in enumerate(paths):
    g4sipm = pickle.load(open(os.path.join(os.path.dirname(path), "g4sipm.pkl")))
    n_particles = int(g4sipm["particleSourceMessenger"]["nParticles"])
    name = g4sipm["sipmModel"]["name"]
    v_ov = g4sipm["sipmModel"]["overVoltage"]
    # Read cached results from tsv file
    theta, n = np.loadtxt(path, delimiter=" ").transpose()  # rad, #
    # Determine bin width
    theta = np.rad2deg(theta)
    theta_min = 0
    theta_max = 90
    nbins = int((theta_max - theta_min) / args.bin_width) + 1
    x, y, yerr, n = histogram.profile(theta, n, name, nbins, 0, 90 + args.bin_width)
    y_max = max(y)
    # Normalize to one.
    y /= y_max
    yerr /= y_max
    # Plot bin statistics.
    label = ("%s +%.1f V" % (name, v_ov * 1e6))
    # Plot number of entries per bin
    ax1.plot(x, n, '.', label=label)
    # Plot relative PDE
    ax2.errorbar(x, y * 100.0, yerr=yerr * 100.0, fmt=".", label=label)

# Plot theoretical assumption
# Index of refraction for dry air (PDG) at 20 degree celsius, 1 atm.
n1 = 1. + 172. * 1e-6
# Index of refraction of the resin.
n2 = 1.5
# Index of refraction for Si at 390 nm.
n3 = complex(4.6869, 0.087976)
# Plot Fresnel analytical solution.
theta = np.linspace(0, 90, 1000)
relative_pde = fresnel.multi(n1, n2, n3, theta * np.pi / 180.)
relative_pde /= relative_pde[0]  /100.0
ax2.plot(theta, relative_pde, color="k", label="Fresnel analytical @ 390 nm")

# Set legend titles.
ax1.set_xlabel("theta / degrees")
ax2.set_xlabel("theta / degrees")
ax1.set_ylabel("entries / %d degrees" % args.bin_width)
ax2.set_ylabel("relative photon detection efficiency / %")

ax2.set_ylim(0, 110)

# Copyright
# ax.text(0.975, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="right", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)

# Create legends
ax1.legend(loc="upper left")
ax2.legend(loc="lower left")

# Tight layout
if args.tight_layout:
    f1.tight_layout()
    f2.tight_layout()

# Save plots
f1.savefig("%s/entries-vs-theta.pdf" % args.path)
f2.savefig("%s/relative-pde-vs-theta.pdf" % args.path)

plt.show()
