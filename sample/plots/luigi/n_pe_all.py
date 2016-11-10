#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob, pickle
import numpy as np
import numpy.ma as ma
import sqlite3
import matplotlib.pyplot as plt
from analysis import crosstalk, darknoise
from contrib import legend, natsort
from ROOT import TH1D, TF1
import ROOT

ROOT.Math.MinimizerOptions.SetDefaultMinimizer("Minuit2")

class FitResult:
    def __init__(self, size):
        self._y = ma.array(np.zeros(size))
        self._yerr = ma.array(np.zeros(size))
        self._chi2 = ma.array(np.zeros(size))
        self._ndof = ma.array(np.zeros(size))
        
    def mask(self, idx):
        self._y[idx] = ma.masked
        self._yerr[idx] = ma.masked
        self._chi2[idx] = ma.masked
        self._ndof[idx] = ma.masked

    def mask_if_invalid(self, fit, idx):
        if fit.GetChisquare() / fit.GetNDF() > 3:
            self.mask(idx)
            return
        # Iterate parameters and check for NaN.
        n = fit.GetNpar()
        for i in xrange(n):
            if np.isnan(fit.GetParameter(i)) or np.isnan(fit.GetParError(i)):
                self.mask(idx)
    
    @property
    def y(self):
        return self._y
    
    @property
    def yerr(self):
        return self._yerr
    
    @property
    def chi2(self):
        return self._chi2
    
    @property
    def ndof(self):
        return self._ndof

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in p.e.", default=1, type=float)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
parser.add_argument("--mod-erlang", help="do a modified Erlang fit", action="store_true", default=False)
args = parser.parse_args()
    
paths = natsort.sort(glob.glob(args.path + "/*/g4sipm.tsv"))

# Prepare figures
f1, ax1 = plt.subplots()
f2, ax2 = plt.subplots()
f3, ax3 = plt.subplots()

# Prepare fit results
x = np.zeros(len(paths))
pois, gaus, erlang, mod_erlang = FitResult(len(paths)), FitResult(len(paths)), FitResult(len(paths)), FitResult(len(paths))

# Iterate files.
for i, path in enumerate(paths):
    # Get SiPM properties
    g4sipm = pickle.load(open(os.path.dirname(path) + "/g4sipm.pkl"))
    n_particles = g4sipm["particleSourceMessenger"]["nParticles"]
    crosstalkNeighbours = g4sipm["g4sipmUiMessenger"]["noiseCrosstalkNeighbours"]
    name = g4sipm["sipmModel"]["name"]
    pde = g4sipm["sipmModel"]["pdeAt400nm"]
    p_ct = g4sipm["sipmModel"]["crossTalkProbability"]
    ncells = g4sipm["sipmModel"]["numberOfCells"]
    v_ov = g4sipm["sipmModel"]["overVoltage"]
    x[i] = n_particles

    # Read cached results from tsv file
    pe = np.loadtxt(path, delimiter=" ")[:, 1]  # number of photons, peak height / p.e.
    # Histogram the number of fired cells.
    xmin = np.floor(np.min(pe))
    xmax = np.ceil(np.max(pe))
    nbins = int((xmax - xmin) / args.bin_width) 
    # Create and fill histogram.
    h = TH1D(name + "-%d" % n_particles, name, nbins, xmin - args.bin_width / 2.0, xmax - args.bin_width / 2.0)
    # h = TH1D(name + "-%d" % n_particles, name, nbins , 0, xmax)
    h.Sumw2()
    for p in pe:
        h.Fill(p)
    
    # Fit a Poisson function
    fit = TF1("fit", "[1] * TMath::Poisson(x, [0])", xmin, xmax)
    fit.SetParameter(0, h.GetMean())
    fit.SetParameter(1, h.GetEntries())
    h.Fit(fit, "0R")
    # Remember fit result.
    pois.y[i] = fit.GetParameter(0)
    pois.yerr[i] = fit.GetParError(0)
    pois.chi2[i] = fit.GetChisquare()
    pois.ndof = fit.GetNDF()
    pois.mask_if_invalid(fit, i)

    # Fit a Erlang distribution
    fit = TF1("fit", darknoise.erlang, xmin, xmax, 3)
    fit.SetParameter(0, h.GetEntries())
    fit.SetParameter(1, h.GetMean() * 1.3)
    fit.SetParameter(2, 1.3)
    fit.SetParLimits(1, 1.0, 10.0 * h.GetMean())
    fit.SetParNames("A", "k", "#lambda")
    h.Fit(fit, "0R")
    # Remember fit result.
    erlang.y[i] = fit.GetParameter(1) / fit.GetParameter(2)
    erlang.yerr[i] = np.sqrt((erlang.y[i] / fit.GetParameter(1) * fit.GetParError(1)) ** 2 + (erlang.y[i] / fit.GetParameter(2) * fit.GetParError(2)) ** 2)
    erlang.chi2[i] = fit.GetChisquare()
    erlang.ndof = fit.GetNDF()
    erlang.mask_if_invalid(fit, i)
    
    # Fit a Gaus distribution
    fit = TF1("fit", "gaus", xmin, xmax)
    fit.SetParameter(0, h.GetEntries())
    fit.SetParameter(1, h.GetMean())
    fit.SetParameter(2, h.GetRMS())
    fit.SetParLimits(1, xmin, xmax)
    h.Fit(fit, "0R")
    # Remember fit result.
    gaus.y[i] = fit.GetParameter(1)
    gaus.yerr[i] = fit.GetParError(1)
    gaus.chi2[i] = fit.GetChisquare()
    gaus.ndof = fit.GetNDF()
    gaus.mask_if_invalid(fit, i)
    if x[i] < 30:
        gaus.mask(i)

    # Fit a modified Erlang distribution (see FACT paper).
    if args.mod_erlang:
        fit = TF1("fit", darknoise.mod_erlang, xmin, xmax, 4)
        fit.SetParameter(0, h.GetEntries())
        fit.SetParameter(1, h.GetMean() * 1.5)
        fit.SetParameter(2, 1.5)
        fit.SetParameter(3, 1.0)
        fit.SetParLimits(1, 1.0, 10.0 * h.GetMean())
        fit.SetParNames("A", "k", "#lambda", "#nu")
        h.Fit(fit, "0R")
        # Remember fit result.
        mod_erlang.y[i] = fit.GetParameter(1) / fit.GetParameter(2)
        mod_erlang.yerr[i] = np.sqrt((mod_erlang.y[i] / fit.GetParameter(1) * fit.GetParError(1)) ** 2 + (mod_erlang.y[i] / fit.GetParameter(2) * fit.GetParError(2)) ** 2)
        mod_erlang.chi2[i] = fit.GetChisquare()
        mod_erlang.ndof = fit.GetNDF()
        mod_erlang.mask_if_invalid(fit, i)

# Theoretical assumption based on simple exp. triggered cells model.
def p(x, y):
    return -ncells / x * np.log(1.0 - y / ncells) * (1 - p_ct)
def nt(x):
    p = pde / (1 - p_ct)
    return ncells * (1.0 - np.exp(-p * x / ncells))

# Plot
ax1.errorbar(x, erlang.y / x, yerr=erlang.yerr / x, fmt=".", label="Erlang fit")
ax1.errorbar(x, pois.y / x, yerr=pois.yerr / x, fmt=".", label="Poisson fit")
ax1.errorbar(x, gaus.y / x, yerr=gaus.yerr / x, fmt=".", label="Gaus fit")

ax2.plot(x, erlang.chi2 / erlang.ndof, ".", label="Erlang fit")
ax2.plot(x, pois.chi2 / pois.ndof, ".", label="Poisson fit")
ax2.plot(x, gaus.chi2 / gaus.ndof, ".", label="Gaus fit")
    
ax3.errorbar(x, p(x, erlang.y) * 100.0, fmt=".", label="Erlang fit")
ax3.errorbar(x, p(x, pois.y) * 100.0, fmt=".", label="Poisson fit")
ax3.errorbar(x, p(x, gaus.y) * 100.0, fmt=".", label="Gaus fit")

if args.mod_erlang:
    ax1.errorbar(x, mod_erlang.y / x, yerr=mod_erlang.yerr / x, fmt=".", label="Modified Erlang fit")
    ax2.plot(x, mod_erlang.chi2 / mod_erlang.ndof, ".", label="Erlang fit")
    ax3.errorbar(x, p(x, mod_erlang.y) * 100.0, fmt=".", label="Modified Erlang fit")

# Plot theoretical assumptions.
ax1.plot(x, nt(x) / x, '-', label="Theoretical assumption")
ax3.plot(x, p(x, nt(x)) * 100.0, '-', label="Theoretical assumption")

ax1.set_xlabel(r"number of photons $N_\gamma$")
ax2.set_xlabel(r"number of photons $N_\gamma$")
ax3.set_xlabel(r"number of photons $N_\gamma$")
ax1.set_ylabel(r"$\mu / N_\gamma$")
ax2.set_ylabel(r"$\chi^2 / NDOF$")
ax3.set_ylabel(r"PDE / %")

ax1.legend(title=name)
ax2.legend(title=name)
ax3.legend(title=name)

if args.tight_layout:
    f1.tight_layout()
    f2.tight_layout()
    f3.tight_layout()

f1.savefig("%s/n-pe-mean-over-n_gamma.pdf" % args.path)
f2.savefig("%s/n-pe-chi2-over-ndof.pdf" % args.path)
f3.savefig("%s/n-pe-pde.pdf" % args.path)

plt.show()
