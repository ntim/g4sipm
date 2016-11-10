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
parser.add_argument("--bin-width", help="the bin width", default=400, type=int)
parser.add_argument("--std-max", help="reconstruction error maximum", default=0.05, type=float)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
parser.add_argument("--log", help="display log scale", action="store_true", default=False)
parser.add_argument("--pde", help="reference pde for best case model", default=0.3, type=float)
args = parser.parse_args()

# Prepare plot
f1, ax1 = plt.subplots()
ax1.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
ax1.set_yscale("log")

# Prepare plot
f2, ax2 = plt.subplots()
ax2.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
ax2.ticklabel_format(axis='y', style='sci', scilimits=(-2, 2))

# Prepare plot
f3, ax3 = plt.subplots()
ax3.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
ax3.ticklabel_format(axis='y', style='sci', scilimits=(-2, 2))

# Prepare plot
f4, ax4 = plt.subplots()
ax4.ticklabel_format(axis='x', style='sci', scilimits=(-2, 2))
ax4.ticklabel_format(axis='y', style='sci', scilimits=(-2, 2))

if args.log:
    for ax in [ax1, ax2, ax3, ax4]:
        ax.set_xscale("log", nonposx='clip')

paths = glob.glob(args.path + "/*/")
paths.sort()
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
    n_c_max = max(n_c)
    x, y, yerr, n = histogram.profile(n_c, n_phot, name, n_c_max / args.bin_width, 0, n_c_max, args.log)
    # Plot bin statistics.
    label = ("%s +%.3f V, T = %.1f $^\circ$C" % (name, v_ov * 1e6, temperature))
    ax1.plot(x, n, '.', label=label)
    # Plot number of photons
    ax2.errorbar(x, y, yerr=yerr, fmt=".", label=label)
    # Plot reconstruction error.
    ax3.plot(x, yerr / y, '.', label=label)
    # Plot reconstruction error vs number of photons.
    ax4.plot(y, yerr / y, '.', label=label)

for ax in [ax3, ax4]:
    # Set axis limits
    if not args.log:
        ax.set_xlim(xmin=0)
    else:
        ax.set_xlim(xmin=1)
    xlim = ax.get_xlim()
    # Plot reconstruction error comparison
    x = np.linspace(xlim[0], xlim[1], 1024)
    # ax.plot(x, np.sqrt(args.pde * x) / x, label="best case for %d %% PDE" % (args.pde * 100.0), color="black")
    # Set axis limits
    ax.set_ylim(ymax=args.std_max * 2.0)
    ylim = ax.get_ylim()
    ax.hlines(args.std_max, xlim[0], xlim[1], color="gray")
    ax.fill_between([xlim[0], xlim[1]], [args.std_max, args.std_max], [ylim[1], ylim[1]], color="gray", alpha=0.2)

# Set legend titles.
ax1.set_xlabel("number of eff. trig. cells $<N_{cell}>$ / %d ns" % int(pulse_width))
ax2.set_xlabel("number of eff. trig. cells $<N_{cell}>$ / %d ns" % int(pulse_width))
ax3.set_xlabel("number of eff. trig. cells $<N_{cell}>$ / %d ns" % int(pulse_width))
ax4.set_xlabel("mean number of photons $<N_\gamma>$ / %d ns" % int(pulse_width))
ax1.set_ylabel("entries")
ax2.set_ylabel("mean number of photons $<N_\gamma>$")
ax3.set_ylabel("relative reconstruction error $\sigma_{\gamma}$ / $<N_\gamma>$")
ax4.set_ylabel("relative reconstruction error $\sigma_{\gamma}$ / $<N_\gamma>$")

# Create legends
ax1.legend(loc="upper left")
ax2.legend(loc="upper left")
ax3.legend(loc="upper right")
ax4.legend(loc="upper right")

# Copyright
# ax1.text(0.975, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="right", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)
# ax2.text(0.975, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="right", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)
# ax3.text(0.025, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="left", va="bottom", fontsize="medium", color="gray", transform=ax2.transAxes)
# ax4.text(0.025, 0.025, "G4SiPM, Tim Niggemann et.al., NIMA 2014", ha="left", va="bottom", fontsize="medium", color="gray", transform=ax1.transAxes)

# Tight layout
if args.tight_layout:
    f1.tight_layout()
    f2.tight_layout()
    f3.tight_layout()
    f4.tight_layout()

# Save plots
if not args.log:
    f1.savefig("%s/entries-vs-n_eff-%d.pdf" % (args.path, int(pulse_width)))
    f2.savefig("%s/n_gamma-vs-n_eff-%d.pdf" % (args.path, int(pulse_width)))
    f3.savefig("%s/relative-error-vs-n_eff-%d.pdf" % (args.path, int(pulse_width)))
    f4.savefig("%s/relative-error-vs-n_phot-%d.pdf" % (args.path, int(pulse_width)))
else:
    f1.savefig("%s/entries-vs-n_eff-%d-log.pdf" % (args.path, int(pulse_width)))
    f2.savefig("%s/n_gamma-vs-n_eff-%d-log.pdf" % (args.path, int(pulse_width)))
    f3.savefig("%s/relative-error-vs-n_eff-%d-log.pdf" % (args.path, int(pulse_width)))
    f4.savefig("%s/relative-error-vs-n_phot-%d-log.pdf" % (args.path, int(pulse_width)))

plt.show()
