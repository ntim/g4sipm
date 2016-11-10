#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os, sys, glob
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from analysis import afterpulse
from contrib import legend
from ROOT import TH1D

parser = argparse.ArgumentParser()
parser.add_argument("path", help="the path to the luigi simulation results directory containing the g4sipm.tsv")
parser.add_argument("--bin-width", help="the bin width in nanoseconds", default=10, type=int)
parser.add_argument("--xmax", help="the maximum x to display in nanoseconds", default=5e3, type=int)
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
parser.add_argument("--scatter-plot", help="display scatter plot instead of 2d color plot", action="store_true", default=False)
args = parser.parse_args()

paths = glob.glob(args.path + "/*/g4sipm-delta-t.tsv")
paths.sort()
for i, path in enumerate(paths):
    # Prepare plots
    f1, ax1 = plt.subplots()
    f2, ax2 = plt.subplots()
    ax2.set_yscale("log")
    
    # Get SiPM properties
    con = sqlite3.connect(glob.glob(os.path.dirname(path) + "/*.sqlite")[0])
    cur = con.cursor()
    n_particles = cur.execute("SELECT nParticles FROM particleSourceMessenger;").fetchone()[0]
    name, v_ov, p_ap_l, p_ap_s, tau_l, tau_s, f_th = cur.execute("SELECT name, overVoltage, apProbLong, apProbShort, apTauLong, apTauShort, thermalNoiseRate FROM sipmModel;").fetchone()
    con.close()
      
    # Read cached results from tsv file
    time_diff, pe = np.loadtxt(path, delimiter=" ").transpose()  # time difference ns, peak height / p.e.
    
    # Plot peak height as function of the time difference
     # Plot the same as in ax1 but as 2d histogram.
    if args.scatter_plot:
        ax1.plot(time_diff, pe, '.', markersize=2, label="entries %d" % len(time_diff))
    else:
        nbins = int(args.xmax / args.bin_width)
        counts, xedges, yedges, image = ax1.hist2d(time_diff, pe, bins=(nbins, 12 / 0.05), range=((0, args.xmax), (0, 12)), norm=LogNorm(), label="entries %d" % len(time_diff))
        cb = f1.colorbar(image)
        cb.set_label("entries")
    
    # Histogram the time difference.
    nbins = int(args.xmax / args.bin_width)
    h = TH1D(name, name, nbins , 0, args.xmax)
    h.Sumw2()
    for ti in time_diff:
        h.Fill(ti)
    # Normalize
    h.Scale(1.0 / h.Integral("width"))
    #
    x = np.array([h.GetBinLowEdge(i) for i in xrange(1, nbins + 1)])
    y = np.array([h.GetBinContent(i) for i in xrange(1, nbins + 1)])
    yerr = np.array([h.GetBinError(i) for i in xrange(1, nbins + 1)])
    ax2.errorbar(x, y, xerr=h.GetBinWidth(1) / 2.0, yerr=yerr, fmt=".", label="entries %d" % len(time_diff))
    
    # Plot the model values.
#     legend.add(ax2,
#                labels=[r"$ P_{ap,s,lit}$", r"$ P_{ap,l,lit}$", r"$\tau_{s,lit}$", r"$\tau_{l,lit}$", r"$\tau_{th,lit}$"],
#                values=[p_ap_s * 100, p_ap_l * 100, tau_s, tau_l, (1.0 / f_th)],
#                units=["%", "%", "ns", "ns", "ns"],
#                loc="lower left",
#                offset=[0.45, 0])
    
    # Fit the time difference.
    amp, amp_err, p_ap_s, p_ap_s_err, p_ap_l, p_ap_l_err, tau_s, tau_s_err, tau_l, tau_l_err, tau_th, tau_th_err, chi2, ndof = afterpulse.fit(h)
    # Plot the fit result.
    legend.add(ax2,
               labels=[r"$\chi^2$/ ndf", "A", r"$ P_{ap,s}$", r"$ P_{ap,l}$", r"$\tau_{s}$", r"$\tau_{l}$", r"$\tau_{th}$"],
               values=[chi2, amp, p_ap_s * 100, p_ap_l * 100, tau_s, tau_l, tau_th],
               errors=[None, amp_err, p_ap_s_err * 100, p_ap_l_err * 100, tau_s_err, tau_l_err, tau_th_err],
               units=["/ %d" % ndof, None, "%", "%", "ns", "ns", "ns"],
               loc="lower left")
    ax2.plot(x, afterpulse.prob_dist([x], [amp, p_ap_s, p_ap_l, tau_s, tau_l, tau_th]), '-', color="red", label="fit")
    
    # Style the plot
    for ax in [ax1, ax2]:
        ax.set_xlabel("time after trigger / ns")
        ax.legend(loc="upper right", title=("%s +%.1f V" % (name, v_ov * 1e6)))
        ax.set_xlim(0, args.xmax)
    ax1.set_ylabel("p.e.")
    ax2.set_ylabel("entries / %d ns" % args.bin_width)
    
    # Tight layout
    if args.tight_layout:
        f1.tight_layout()
        f2.tight_layout()
    
    # Save plots
    f1.savefig("%s/%s-pe-vs-delta_t.pdf" % (args.path, name))
    f2.savefig("%s/%s-entries-vs-delta_t.pdf" % (args.path, name))

plt.show()
