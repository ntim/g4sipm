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
hits = persistency.get_multiple(fp, "G4SipmHit")

t = []
for h in hits:
    t.append(h['time'])

t = np.asarray(t)
bins = np.max(t) - np.min(t)
bins = max(100, bins)

plt.hist(t, bins=bins, histtype="stepfilled", ec="#1f77b4", fc="#1f77b4")

plt.xlabel('t / ns')
plt.ylabel('entries')
plt.show()
