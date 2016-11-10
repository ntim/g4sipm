#!/usr/bin/python
#
#
#

import os, sys, numpy as np 
import matplotlib.pyplot as plt
from lseq import *
import sqlite3
from scipy.interpolate import interp1d

path = "./results/dnr"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = os.listdir(path)
files.sort()

photons = []
weights = []
pulseWidth = None
sipmModel = None
pde = 0
pct = 0
papl = 0
paps = 0
nCell = 0
integrationWindow = 1000
for file in files:
    if file[-7:] == '.sqlite':
        print file
        con = sqlite3.connect(path + "/" + file)
        cur = con.cursor()
        if pulseWidth == None or sipmModel == None:
            # Get model name.
            sipmModel = cur.execute("SELECT name FROM sipmModel;").fetchone()[0]
            pde, pct, papl, paps, nCell = cur.execute("SELECT pdeAt400nm, crosstalkProbability, apProbLong, apProbShort, numberOfCells FROM sipmModel;").fetchone()
            # Get time window of the simulation.
            tMin, tMax, pulseWidth = cur.execute("SELECT tMin, tMax, (tMax-tMin) FROM particleSourceMessenger;").fetchone()
        # Get number of particles.
        # Compute sum of weights.
        try:
            sum = 0
            for d in cur.execute("SELECT weight FROM `g4sipmDigis-0` WHERE time BETWEEN %g AND %g;" % (tMin, tMin + integrationWindow)):
                sum += d[0]  # * (1.5-sum*0.375e-3)/1.5
            if sum > 0:
                weights.append(sum)
                photons.append(cur.execute("SELECT nParticles FROM particleSourceMessenger;").fetchone()[0])
        except:
            pass
                               
x = np.asarray(photons)
y = np.asarray(weights)
bins = lseq(1, max(x), 100)

# Compute histogram with mean and std per bin.
n, _ = np.histogram(x, bins=bins)
sy, _ = np.histogram(x, bins=bins, weights=y)
sy2, _ = np.histogram(x, bins=bins, weights=y * y)
mean = sy / n
std = np.sqrt(sy2 / n - mean * mean)

# Plot
plt.xscale("log")
plt.yscale("log")

#plt.plot((_[1:] + _[:-1]) / 2, mean, linewidth=2, label="G4Sipm simulation")
plt.plot(x, y, ".", color="red")

# Plot theoretical assumption.
#prop = pde * (1.0 + pct + (1 - (1 - paps) * (1 - papl)))
#x = np.linspace(meas[0][0], meas[0][-1], 1000)
#plt.plot(x, nCell * (1 - np.exp(-x * prop / nCell)), '-', linewidth=2, label="rule of thumb")

# Annotations.
plt.text(0.975, 0.05, sipmModel, ha="right", va="top", fontsize="medium", transform=plt.axes().transAxes)

plt.xlabel("number of photons / %d ns pulse" % int(pulseWidth))
plt.ylabel("number of eff. cells triggered $n^{eff}$ in %d ns" % int(integrationWindow))
# plt.legend(loc="upper left")
#plt.xlim(meas[0][0]/2., meas[0][-1]*2.)
plt.xlim(min(x), max(x))
plt.ylim(min(y), max(y))
# plt.ylim(np.max(mean)*2., np.min(mean)/2.)
plt.savefig("dnr2-%d-%d.pdf" % (int(pulseWidth), int(integrationWindow)))

plt.show()
