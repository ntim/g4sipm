#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Plots the distribution of the photon hits into a 2d-histogram.
# 

import sys, os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import sqlite3
import glob

filename = glob.glob('results/*.sqlite')[-1]
if len(sys.argv) > 1:
    filename = sys.argv[1]
    
filter = None

con = sqlite3.connect(filename)
cur = con.cursor()
name, pitch, cellPitch, fillFactor, numberOfCells = cur.execute("SELECT name, pitch, cellPitch, fillFactor, numberOfCells FROM sipmModel;").fetchone()

range = [-pitch / 2., pitch / 2.]
bins = []
current = -pitch / 2.
edge = cellPitch * (1. - np.sqrt(fillFactor)) / 2.
cell = cellPitch * np.sqrt(fillFactor)
for i in xrange(0, int(np.sqrt(numberOfCells))):
    bins.append(current)
    for c in np.linspace(current + edge, current + edge + cell, 10):
        bins.append(c)
    current += cellPitch
bins.append(current)

hits = cur.execute("SELECT position FROM `g4sipmHits-0`;").fetchall()
x, y = [], []
for h in hits:
    xi, yi, _ = h[0][1:-1].split(",")
    x.append(float(xi))
    y.append(float(yi))
    
H, xedges, yedges = np.histogram2d(x, y, bins=(bins, bins), range=[range, range])
X, Y = np.meshgrid(xedges, yedges)
plt.pcolor(X, Y, H, cmap=cm.Reds, vmax=np.max(H), vmin=0)

plt.text(0.025, 0.975, "%s\n%d x %d mm, %d $\mu$m pitch\n%d cells"  % (name, pitch, pitch, cellPitch * 1000, numberOfCells), 
         ha="left", va="top", fontsize="medium", transform=plt.axes().transAxes)

plt.xlabel('x / mm')
plt.ylabel('y / mm')
plt.xlim(range)
plt.ylim(range)
plt.savefig("hits.pdf")
plt.show()
