#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob, pickle
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from analysis import crosstalk, darknoise
from contrib import legend
from ROOT import TH1D, TF1
import ROOT

ROOT.Math.MinimizerOptions.SetDefaultMinimizer("Minuit2")

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in p.e.", default=0.1, type=float)
parser.add_argument("--xmax", help="the maximum x to display in p.e.", default=12, type=int)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

path = args.path + "/g4sipm-pe.tsv"
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
h = TH1D(name, name, nbins , 0 + args.bin_width / 2.0, args.xmax + args.bin_width / 2.0)
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
ylim = ax1.get_ylim()

# Fit
func = TF1("Spectrum", darknoise.single_pe_spectrum, 0 + args.bin_width / 2.0, args.xmax + args.bin_width / 2.0, 7);
func.SetParameter(0, max(y))  # peak
func.SetParameter(1, 1.02259)  # gain
func.SetParameter(2, 0.0955702)  # sigma
func.SetParameter(3, 0.0974085)  # crosstalk
func.SetParameter(4, -0.0257501)  # shift
func.SetParameter(5, 0.0339967)  # noise
func.SetParameter(6, 1.22562)  # expo
h.Fit(func, "LNOR")

p_ct_fit, p_ct_fit_std = darknoise.compute_crosstalk_probability(func.GetParameters(), func.GetParErrors())
edge_correction_factor = crosstalk.edge_correction_factor(ncells, crosstalkNeighbours)
p_ct_fit /= edge_correction_factor
p_ct_fit_std /= edge_correction_factor

# Plot fit result.
x_fit = np.linspace(0 + args.bin_width / 2.0, args.xmax + args.bin_width / 2., 1024)
y_fit = [func.Eval(xi) for xi in x_fit]
ax1.plot(x_fit, y_fit, '-', color="red", label="fit")
ax1.set_ylim(ylim)

# Plot the fit result.
legend.add(ax1,
        labels=[r"$ P_{ct}$", r"$\kappa(%d)$" % crosstalkNeighbours, r"$ P_{ct,lit}$", r"$\chi^2 / ndof$", "g", r"$\sigma_{pe}$", r"$ P_{ct}$", r"$\sigma_{el}$", r"$\nu$"],
        values=[p * 100.0, edge_correction_factor, p_ct * 100.0, func.GetChisquare(), func.GetParameter(1), func.GetParameter(2), p_ct_fit * 100.0, func.GetParameter(5), func.GetParameter(6)],
        errors=[std * 100.0, None, None, None, func.GetParError(1), func.GetParError(2), p_ct_fit_std * 100.0, func.GetParError(5), func.GetParError(6)],
        units=[r"%", None, None, "/ %d" % func.GetNDF(), None, None, None, None, None],
        loc="upper right",
        title=("%s +%.1f V" % (name, v_ov * 1e6)))

# Style the plot
ax1.set_xlabel("p.e.")
ax1.set_ylabel("entries / %.1f p.e." % args.bin_width)
ax1.legend(loc="lower right")

# Tight layout
if args.tight_layout:
    f1.tight_layout()

# Save plots
f1.savefig("%s/../%s-%d-entries-vs-pe.pdf" % (args.path, name, crosstalkNeighbours))

plt.show()
