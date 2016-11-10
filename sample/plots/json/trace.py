#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, glob
import persistency
import matplotlib.pyplot as plt
import numpy as np

millivolt = 1e-9
	
if __name__ == "__main__":
	filename = glob.glob('results/*.txt')[-1]
	if len(sys.argv) > 1:
		filename = sys.argv[1]
	# Plot range from arguments
	start = 0
	stop = 1000
	if len(sys.argv) > 3:
		start = int(sys.argv[2])
		stop = start + int(sys.argv[3])
	# Open file.
	fp = open(filename)
	digi = persistency.get(fp, "G4SipmVoltageTraceDigi")
	model = persistency.get(fp, "G4SipmModel")
	# Extract trace.
	voltages = []
	times = []
	for i, v in enumerate(digi["voltages"]):
		t = digi["tMin"] + i * digi["timeBinWidth"]
		if t >= start and t <= stop:
			voltages.append(v / millivolt)
			times.append(t / 1000.0)
	# Plot traces superposed.
	plt.plot(times, voltages, '-')
	#
	plt.text(0.025, 0.975, "%s\n%d x %d mm, %d $\mu$m pitch\n%d cells"  % (model["model"], model["pitch"], model["pitch"], model["cellPitch"] * 1000, model["numberOfCells"]), 
			ha="left", va="top", fontsize="medium", transform=plt.axes().transAxes)
	plt.ylabel('voltage / mV')
	plt.xlabel(u'time / µs')
	plt.savefig("trace.pdf")
	plt.show()
