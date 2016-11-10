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
parser.add_argument("--tight-layout", help="display log scale", action="store_true", default=False)
args = parser.parse_args()

paths = glob.glob(args.path + "/*/")
paths.sort()
for i, path in enumerate(paths):
    # Prepare plots
    f1, ax1 = plt.subplots() 
    
    # Get SiPM properties
    g4sipm = pickle.load(open(path + "/g4sipm.pkl"))
    crosstalkNeighbours = g4sipm["g4sipmUiMessenger"]["noiseCrosstalkNeighbours"]
    name = g4sipm["sipmModel"]["name"]
    p_ct = g4sipm["sipmModel"]["crossTalkProbability"]
    ncells = g4sipm["sipmModel"]["numberOfCells"]
    v_ov = g4sipm["sipmModel"]["overVoltage"]
    
    # Histogram position on the SiPM
    pitch = int(np.sqrt(ncells))
    x, y, w = [], [], []
    
    # Open a sqlite file
    for db in glob.glob(path + "/*.sqlite"):
        con = sqlite3.connect(db)
        cur = con.cursor()
        cur.execute("SELECT cellid, weight FROM `g4sipmDigis-0` WHERE type = 3")
        for row in cur:
            x.append(row[0] % pitch)
            y.append(row[0] / pitch)
            w.append(row[1])
        
    # Histogram.
    h, xedges, yedges, image = ax1.hist2d(x, y, bins=pitch, range=((0, pitch), (0, pitch)), weights=w, label="entries %d" % len(x))
    cb = f1.colorbar(image)
    cb.set_label("entries")
    # Scale histogram to the maximum number of neighbours
    # h.Scale(crosstalkNeighbours / h.GetMaximum())
    # Draw

plt.show()
