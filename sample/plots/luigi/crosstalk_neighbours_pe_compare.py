#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob
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
# Prepare plots
f1, ax1 = plt.subplots() 
ax1.set_yscale("log")
f2, ax2 = plt.subplots()

h_ref = None
    
for i, path in enumerate(paths):
    # Get SiPM properties
    con = sqlite3.connect(glob.glob(os.path.dirname(path) + "/*.sqlite")[0])
    cur = con.cursor()
    try:
        crosstalkNeighbours = cur.execute("SELECT noiseCrosstalkNeighbours FROM g4sipmUiMessenger;").fetchone()[0]
    except:
        print "Fallback to crosstalkNeighbours = 4"
        crosstalkNeighbours = 4
    n_particles = cur.execute("SELECT nParticles FROM particleSourceMessenger;").fetchone()[0]
    name, p_ct, ncells, v_ov = cur.execute("SELECT name, crosstalkProbability, numberOfCells, overVoltage FROM sipmModel;").fetchone()
    con.close()      
      
    # Read cached results from tsv file
    pe = np.loadtxt(path, delimiter=" ").transpose()  # time difference ns, peak height / p.e.
    
    # Histogram the time difference.
    nbins = int(args.xmax / args.bin_width)
    # Create and fill histogram.
    h = TH1D(name + "-%d" % i, name + "-%d" % i, nbins , 0 + args.bin_width / 2.0, args.xmax + args.bin_width / 2.0)
    h.Sumw2()
    for p in pe:
        h.Fill(p)
    # Normalize to one.
    h.Scale(1.0 / h.Integral("width"))
    if h_ref == None:
        h_ref = h
    #
    x = np.array([h.GetBinLowEdge(i) for i in xrange(1, nbins + 1)])
    y = np.array([h.GetBinContent(i) for i in xrange(1, nbins + 1)])
    yerr = np.array([h.GetBinError(i) for i in xrange(1, nbins + 1)])
    # Compute crosstalk probability.
    p, std = crosstalk.prob(h, ncells, crosstalkNeighbours)
    
    # Plot
    ax1.hist(x, bins=x, weights=y, histtype="step", label="entries %d, $ n_{ct}=%d$" % (len(pe), crosstalkNeighbours))
    ax1.errorbar(x + h.GetBinWidth(1) / 2.0, y, yerr=yerr, fmt='.', color="k", capthick=0)
    
    # Plot comparison plot
    h.Divide(h_ref)
    x = np.array([h.GetBinLowEdge(i) for i in xrange(1, nbins + 1)])
    y = np.array([h.GetBinContent(i) for i in xrange(1, nbins + 1)])
    yerr = np.array([h.GetBinError(i) for i in xrange(1, nbins + 1)])
    ax2.hist(x, bins=x, weights=y, histtype="step", label="entries %d, $ n_{ct}=%d$" % (len(pe), crosstalkNeighbours))
    ax2.errorbar(x + h.GetBinWidth(1) / 2.0, y, yerr=yerr, fmt='.', color="k", capthick=0)
    

# Style the plot
ax1.set_xlabel("p.e.")
ax2.set_xlabel("p.e.")
ax1.set_ylabel("fraction of entries / %.1f p.e." % args.bin_width)
ax2.set_ylabel("ratio / %.1f p.e." % args.bin_width)
ax1.legend(loc="upper right", title=("%s +%.1f V" % (name, v_ov * 1e6)))
ax2.legend(loc="upper right", title=("%s +%.1f V" % (name, v_ov * 1e6)))
    
# Tight layout
if args.tight_layout:
    f1.tight_layout()
    
# Save plots
f1.savefig("%s/entries-vs-pe.pdf" % (args.path))
f2.savefig("%s/ratio-vs-pe.pdf" % (args.path))

plt.show()
