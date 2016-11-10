#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Analysis and plot script for the output of "run/relative_pde.py" with the angle of incidence on the x-axis and the photon 
# detection efficiency normalized to zero degrees on the y axis.
#
import os, sys, glob
import matplotlib.pyplot as plt
import numpy as np
import fresnel
import sqlite3

files = glob.glob("./results/relative_pde/*.sqlite")
if len(sys.argv) > 1:
	files = glob.glob(sys.argv[1])

# Read data from the Geant4 simulation.
theta = []
relative_pde = []
for file in files:
    con = sqlite3.connect(file)
    cur = con.cursor()
    eMin, nParticles, surfaceNormal = cur.execute("SELECT eMin, nParticles, surfaceNormal FROM particleSourceMessenger;").fetchone()
    nHits = cur.execute("SELECT COUNT(*) FROM `g4sipmHits-0`;").fetchone()[0]
    #
    surfaceNormalZ = float(surfaceNormal[1:-1].split(',')[2])
    theta.append(180. - np.arccos(surfaceNormalZ) * 180. / np.pi)
    relative_pde.append(nHits)
    
theta = np.asarray(theta)
idx = np.argsort(theta)
theta = theta[idx]
relative_pde = np.asarray(relative_pde, dtype=float)[idx]
relative_pde_err = np.sqrt(relative_pde)

# Scale to entry at nearly zero degree.
zero = np.mean(relative_pde[0:3]) 
# Correction factor for loss of area when rotation the SiPM in the photon beam.
corr = 1. / np.cos(theta * np.pi / 180.)
# Scale values.
relative_pde = relative_pde * corr / zero
# Calculate uncertainty.
relative_pde_sigma = np.sqrt((relative_pde_err * corr / zero) ** 2 + (relative_pde * corr / zero / zero * np.sqrt(zero)) ** 2)

plt.plot(theta, relative_pde, '-', label="G4Sipm simulation")
plt.fill_between(theta, relative_pde - relative_pde_sigma, relative_pde + relative_pde_sigma, color='#1f77b4', alpha=0.2)

# Index of refraction for dry air (PDG) at 20 degree celsius, 1 atm.
n1 = 1. + 172. * 1e-6
# Index of refraction of the resin.
n2 = 1.5
# Index of refraction for Si at 390 nm.
n3 = complex(5.948, 0.561)
# Plot Fresnel analytical solution.
theta = np.linspace(0, 90, 180)
relative_pde = fresnel.multi(n1, n2, n3, theta * np.pi / 180.)
relative_pde /= relative_pde[0]
plt.plot(theta, relative_pde, label="Fresnel analytical @ 390 nm")

name, pitch, cellPitch, numberOfCells = cur.execute("SELECT name, pitch, cellPitch, numberOfCells FROM sipmModel;").fetchone()
plt.text(0.975, 0.975, "%s\n%d x %d mm, %d $\mu$m pitch\n%d cells"  % (name, pitch, pitch, cellPitch * 1000, numberOfCells), 
        ha="right", va="top", fontsize="medium", transform=plt.axes().transAxes)
    
plt.xlabel(r"incident angle $\theta$ in degree")
plt.ylabel("relative PDE / %")
plt.legend(loc=3)
plt.savefig("relative_pde.pdf")
plt.show()
