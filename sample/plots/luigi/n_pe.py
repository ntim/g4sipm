#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob, pickle
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from analysis import crosstalk, darknoise
from contrib import legend, natsort
from ROOT import TH1D, TF1
import ROOT
from matplotlib.backends.backend_pdf import PdfPages

ROOT.Math.MinimizerOptions.SetDefaultMinimizer("Minuit2")

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in p.e.", default=1, type=float)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

pdf = PdfPages(args.path + '/n_pe.pdf')

paths = natsort.sort(glob.glob(args.path + "/*/g4sipm.tsv"))
for i, path in enumerate(paths):
    # Prepare plots
    f1, ax1 = plt.subplots() 
    
    # Get SiPM properties
    g4sipm = pickle.load(open(os.path.dirname(path) + "/g4sipm.pkl"))
    n_particles = g4sipm["particleSourceMessenger"]["nParticles"]
    crosstalkNeighbours = g4sipm["g4sipmUiMessenger"]["noiseCrosstalkNeighbours"]
    name = g4sipm["sipmModel"]["name"]
    pde = g4sipm["sipmModel"]["pdeAt400nm"]
    p_ct = g4sipm["sipmModel"]["crossTalkProbability"]
    ncells = g4sipm["sipmModel"]["numberOfCells"]
    v_ov = g4sipm["sipmModel"]["overVoltage"]

    # Read cached results from tsv file
    pe = np.loadtxt(path, delimiter=" ")[:, 1]  # number of photons, peak height / p.e.
    
    # Histogram the time difference.
    xmin = np.floor(np.min(pe))
    xmax = np.ceil(np.max(pe))
    nbins = int((xmax - xmin) / args.bin_width) 
    # Create and fill histogram.
    h = TH1D(name + "-%d" % n_particles, name, nbins, xmin - args.bin_width / 2.0, xmax - args.bin_width / 2.0)
    # h = TH1D(name + "-%d" % n_particles, name, nbins , 0, xmax)
    h.Sumw2()
    for p in pe:
        h.Fill(p)
    #
    x = np.array([h.GetBinLowEdge(i) for i in xrange(1, nbins + 1)])
    y = np.array([h.GetBinContent(i) for i in xrange(1, nbins + 1)])
    yerr = np.array([h.GetBinError(i) for i in xrange(1, nbins + 1)])
    
    # Plot
    ax1.hist(x, bins=x, weights=y, histtype="step", label="entries %d" % len(pe))
    ax1.errorbar(x + h.GetBinWidth(1) / 2.0, y, yerr=yerr, fmt='.', color="k", capthick=0)
    
    # Fit a Poisson function
    fit = TF1("fit", "[1] * TMath::Poisson(x, [0])", xmin, xmax)
    fit.SetParameter(0, h.GetMean())
    fit.SetParameter(1, h.GetEntries())
    h.Fit(fit, "0R")
    
    # Plot fit result.
    x_fit = np.linspace(xmin, xmax, 1024)
    y_fit = [fit.Eval(xi) for xi in x_fit]
    ax1.plot(x_fit, y_fit, '-', label="Poisson fit")
    
     # Plot the fit result.
    legend.add(ax1,
               labels=[r"$ N_{ph}$", r"Mean", r"$\mu$", r"$\chi^2 / ndof$", r"PDE(400 nm)", r"$ P_{ct}$"],
               values=[n_particles, h.GetMean(), fit.GetParameter(0), fit.GetChisquare(), pde * 100.0, p_ct * 100.0],
               errors=[None, None, fit.GetParError(0), None, None, None],
               units=[None, None, None, "/ %d" % fit.GetNDF(), r"%", r"%"],
               loc="center right",
               title=("%s +%.1f V" % (name, v_ov * 1e6)))
    
    # Fit a Erlang distribution
    fit = TF1("fit", darknoise.erlang, xmin, xmax, 3)
    fit.SetParameter(0, h.GetEntries())
    fit.SetParameter(1, h.GetMean() * 1.3)
    fit.SetParameter(2, 1.3)
    fit.SetParLimits(1, 1.0, 10.0 * h.GetMean())
    fit.SetParNames("A", "k", "#lambda")
    h.Fit(fit, "0R")
    
    # Plot fit result.
    x_fit = np.linspace(xmin, xmax, 1024)
    y_fit = [fit.Eval(xi) for xi in x_fit]
    ax1.plot(x_fit, y_fit, '-', label="Erlang fit")
    
    # Fit a Gaus distribution
    fit = TF1("fit", "gaus", xmin, xmax)
    fit.SetParameter(0, h.GetEntries())
    fit.SetParameter(1, h.GetMean())
    fit.SetParameter(2, h.GetRMS())
    h.Fit(fit, "0R")
    
    # Plot fit result.
    x_fit = np.linspace(xmin, xmax, 1024)
    y_fit = [fit.Eval(xi) for xi in x_fit]
    ax1.plot(x_fit, y_fit, '-', label="Gaus fit")
    
    # Style the plot
    ax1.set_xlabel("p.e.")
    ax1.set_ylabel("entries / %.1f p.e." % args.bin_width)
    ax1.legend(loc="upper right")
    
    # Tight layout
    if args.tight_layout:
        f1.tight_layout()
    
    # Save plots
    pdf.savefig(f1)
    plt.close(f1)

pdf.close()
