#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
import pickle
from contrib import histogram

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width", default=1e4, type=int)
parser.add_argument("--std_max", help="reconstruction error maximum", default=0.05, type=float)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
parser.add_argument("--log", help="display log scale", action="store_true", default=False)
parser.add_argument("--save", help="save histogrammed data to text file", action="store_true", default=False)
args = parser.parse_args()

# Prepare plot
f1, ax1 = plt.subplots()
if args.log:
    ax1.set_xscale("log", nonposx='clip')
else:
    ax1.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
ax1.set_yscale("log")

# Prepare plot
f2, ax2 = plt.subplots()
if args.log:
    ax2.set_xscale("log", nonposx='clip')
    ax2.set_yscale("log", nonposx='clip')
else:
    ax2.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
    ax2.ticklabel_format(axis='y', style='sci', scilimits=(-2, 2))
    
# Prepare plot
f3, ax3 = plt.subplots()
if args.log:
    ax3.set_xscale("log", nonposx='clip')
    ax3.set_yscale("log", nonposx='clip')
else:
    ax3.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
    ax3.ticklabel_format(axis='y', style='sci', scilimits=(-2, 2))

# Prepare plot
f4, ax4 = plt.subplots()
if args.log:
    ax4.set_xscale("log", nonposx='clip')
    ax4.set_yscale("log", nonposx='clip')
else:
    ax4.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
    ax4.ticklabel_format(axis='y', style='sci', scilimits=(-2, 2))


paths = glob.glob(args.path + "/*/")
paths.sort()
ymax = 0.0
for i, path in enumerate(paths):
    if not os.path.exists(path + "/g4sipm.pkl") or not os.path.exists(path + "/g4sipm.tsv"):
        continue
    # Get SiPM properties
    g4sipm = pickle.load(open(path + "/g4sipm.pkl"))
    crosstalkNeighbours = g4sipm["g4sipmUiMessenger"]["noiseCrosstalkNeighbours"]
    name = g4sipm["sipmModel"]["name"]
    pde = g4sipm["sipmModel"]["pdeAt400nm"]
    pct = g4sipm["sipmModel"]["crossTalkProbability"]
    papl = g4sipm["sipmModel"]["apProbLong"]
    paps = g4sipm["sipmModel"]["apProbShort"]
    n_cell = g4sipm["sipmModel"]["numberOfCells"]
    v_ov = g4sipm["sipmModel"]["overVoltage"]
    temperature = g4sipm["sipmModel"]["temperature"] - 273.15
    pulse_width = (g4sipm["particleSourceMessenger"]["tMax"] - g4sipm["particleSourceMessenger"]["tMin"])
    # Read cached results from tsv file
    n_phot, n_c = np.loadtxt(path + "/g4sipm.tsv", delimiter=" ").transpose()
    # Fill TProfile
    n_phot_max = max(n_phot)
    x, y, yerr, n = histogram.profile(n_phot, n_c, name, n_phot_max / args.bin_width, 0, n_phot_max, log=args.log)
    # Save to file. 
    if args.save:
        np.savetxt(path + "/hist.txt", np.array((x, y, yerr, n)).T)
    # Plot bin statistics.
    label = ("%s +%.3f V, T = %.1f $^\circ$C" % (name, v_ov * 1e6, temperature))
    ax1.plot(x, n, '.', label=label)
    # Plot number of photons
    ax2.errorbar(x, y, yerr=yerr, fmt=".", label=label)
    # Remember largest value
    ymax = max(ymax, max(y))
    #
    ax3.errorbar(y / n_cell, y / x, yerr=yerr / x, fmt=".", label=label)
    # Scatter plot
    ax4.plot(n_phot, n_c, ".", label=label)
    # Find the outliers
    #idx = np.where((n_phot > 7.2e5) & (n_phot < 7.3e5) & (n_c < 5000) & (n_c > 4300))
    #print idx

# Set legend titles.
ax1.set_xlabel("mean number of photons $<N_\gamma>$ / %d ns" % int(pulse_width))
ax2.set_xlabel("mean number of photons $<N_\gamma>$ / %d ns" % int(pulse_width))
ax3.set_xlabel("fraction of triggered cells / %d ns" % int(pulse_width))
ax4.set_xlabel("mean number of photons $<N_\gamma>$ / %d ns" % int(pulse_width))
ax1.set_ylabel("entries")
ax2.set_ylabel("number of eff. trig. cells $<N_{cell}>$ / %d ns" % int(pulse_width))
ax3.set_ylabel("PDE / %d ns" % int(pulse_width))
ax4.set_ylabel("number of eff. trig. cells $<N_{cell}>$ / %d ns" % int(pulse_width))

# Copyright
# ax1.text(0.975, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="right", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)
# ax2.text(0.975, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="right", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)

# Create legends
ax1.legend(loc="upper left")
ax2.legend(loc="lower right")
ax3.legend(loc="upper left")
ax4.legend(loc="lower right")

# Tight layout
if args.tight_layout:
    f1.tight_layout()
    f2.tight_layout()
    f3.tight_layout()
    f4.tight_layout()

# Save plots
if args.log:
    ax2.set_ylim(ymax=ymax * 2)
    f1.savefig("%s/entries-vs-n_phot-%d-log.pdf" % (args.path, int(pulse_width)))
    f2.savefig("%s/n_eff-vs-n_gamma-%d-log.pdf" % (args.path, int(pulse_width)))
else:
    f1.savefig("%s/entries-vs-n_phot-%d.pdf" % (args.path, int(pulse_width)))
    f2.savefig("%s/n_eff-vs-n_gamma-%d.pdf" % (args.path, int(pulse_width)))

plt.show()
