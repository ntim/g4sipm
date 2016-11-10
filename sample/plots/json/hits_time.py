#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Histograms the time distribution of the hits.
# 

import sys, os, glob
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import persistency

filename = glob.glob('results/*.txt')[-1]
if len(sys.argv) > 1:
    filename = sys.argv[1]
    
fp = open(filename)
digis = persistency.get_multiple(fp, "G4SipmDigi")
model = persistency.get(fp, "G4SipmModel")

t_signal, t_thermal, t_ct, t_ap = [], [], [], []
for d in digis:
    d['time'] /= 1000
    if d['type'] == 0:
        t_signal.append(d['time'])
    elif d['type'] == 1:
        t_thermal.append(d['time'])
    elif d['type'] == 2:
        t_ct.append(d['time'])
    elif d['type'] == 3:
        t_ap.append(d['time'])
        
t = []
labels = []
if len(t_signal) > 0:
    t.append(np.asarray(t_signal))
    labels.append("photon")
if len(t_thermal) > 0:
    t.append(np.asarray(t_thermal))
    labels.append("thermal")
if len(t_ct) > 0:
    t.append(np.asarray(t_ct))
    labels.append("crosstalk")
if len(t_ap) > 0:
    t.append(np.asarray(t_ap))
    labels.append("afterpulse")
    
bins = 256
range = (0, 10)

colors = "#1f77b4, #ff7f0e, #2ca02c, #d62728".split(", ")[0:len(t)]
plt.hist(t, bins=bins, range=range, stacked=True, histtype="stepfilled", color=colors, linewidth=0, ec="white", label=labels)


plt.text(0.025, 0.975, "%s\n%d x %d mm, %d $\mu$m pitch\n%d cells"  % (model["model"], model["pitch"], model["pitch"], model["cellPitch"] * 1000, model["numberOfCells"]), 
        ha="left", va="top", fontsize="medium", transform=plt.axes().transAxes)

plt.legend(loc="upper right")
plt.xlabel(u't / µs')
plt.ylabel('entries')
plt.show()
