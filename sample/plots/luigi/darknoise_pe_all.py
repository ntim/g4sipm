#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob, pickle
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from analysis import crosstalk
from contrib import legend
from ROOT import TH1D

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in p.e.", default=1, type=float)
parser.add_argument("--xmax", help="the maximum x to display in p.e.", default=12, type=int)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

paths = glob.glob(args.path + "/*/g4sipm-pe.tsv")
paths.sort()
for i, path in enumerate(paths):
    # Prepare plots
    f1, ax1 = plt.subplots() 
    ax1.set_yscale("log")
    
    # Get SiPM properties
    g4sipm = pickle.load(open(os.path.dirname(path) + "/g4sipm.pkl"))
    crosstalkNeighbours = g4sipm["g4sipmUiMessenger"]["noiseCrosstalkNeighbours"]
    name = g4sipm["sipmModel"]["name"]
    p_ct = g4sipm["sipmModel"]["crossTalkProbability"]
    ncells = g4sipm["sipmModel"]["numberOfCells"]
    v_ov = g4sipm["sipmModel"]["overVoltage"]

    # Read cached results from tsv file
    pe = np.loadtxt(path, delimiter=" ").transpose()  # time difference ns, peak height / p.e.
    
    # Histogram the time difference.
    nbins = int(args.xmax / args.bin_width)
    # Create and fill histogram.
    h = TH1D(name + "-%d" % crosstalkNeighbours, name, nbins , 0 + args.bin_width / 2.0, args.xmax + args.bin_width / 2.0)
    h.Sumw2()
    for p in pe:
        h.Fill(p)
    # Normalize to one.
    # h.Scale(1.0 / h.Integral("width"))
    #
    x = np.array([h.GetBinLowEdge(i) for i in xrange(1, nbins + 1)])
    y = np.array([h.GetBinContent(i) for i in xrange(1, nbins + 1)])
    yerr = np.array([h.GetBinError(i) for i in xrange(1, nbins + 1)])
    # Compute crosstalk probability.
    p, std = crosstalk.prob(h, ncells, crosstalkNeighbours)
    
    # Plot
    ax1.hist(x, bins=x, weights=y, histtype="step", label="entries %d" % len(pe))
    ax1.errorbar(x + h.GetBinWidth(1) / 2.0, y, yerr=yerr, fmt='.', color="k", capthick=0)
    
     # Plot the fit result.
    legend.add(ax1,
               labels=[r"$ P_{ct}$", r"$\kappa(%d)$" % crosstalkNeighbours, r"$ P_{ct,lit}$"],
               values=[p * 100.0, crosstalk.edge_correction_factor(ncells, crosstalkNeighbours), p_ct * 100.0],
               errors=[std * 100.0, None, None],
               units=[r"%", None, None],
               loc="center right",
               title=("%s +%.1f V" % (name, v_ov * 1e6)))
    
    # Style the plot
    ax1.set_xlabel("p.e.")
    ax1.set_ylabel("entries / %.1f p.e." % args.bin_width)
    ax1.legend(loc="upper right")
    
    # Tight layout
    if args.tight_layout:
        f1.tight_layout()
    
    # Save plots
    f1.savefig("%s/%s-%d-entries-vs-pe.pdf" % (args.path, name, crosstalkNeighbours))

plt.show()
