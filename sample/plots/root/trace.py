#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, glob
import matplotlib.pyplot as plt
import numpy as np
from ROOT import TFile, TTree

millivolt = 1e-9
		
if __name__ == "__main__":
	filename = glob.glob('results/*.root')[-1]
	if len(sys.argv) > 1:
		filename = sys.argv[1]
	# Plot range from arguments
	start = 0
	stop = 1000
	if len(sys.argv) > 3:
		start = int(sys.argv[2])
		stop = start + int(sys.argv[3])
	# Open file.
	f = TFile(filename)
	digi = f.Get("g4sipmVoltageTraceDigis-0")
	model = f.Get("sipmModel")
	model.GetEntry(0)
	# Extract trace.
	voltages = []
	times = []
	for i in xrange(digi.GetEntries()):
		digi.GetEntry(i)
		if digi.time >= start and digi.time <= stop:
			voltages.append(digi.voltage / millivolt)
			times.append(digi.time / 1000)
	# Plot traces superposed.
	plt.plot(times, voltages, '-')
	#
	plt.text(0.025, 0.975, "%s\n%d x %d mm, %d $\mu$m pitch\n%d cells"  % (model.model, model.pitch, model.pitch, model.cellPitch * 1000, model.numberOfCells), 
			ha="left", va="top", fontsize="medium", transform=plt.axes().transAxes)
	plt.ylabel('voltage / mV')
	plt.xlabel(u'time / µs')
	plt.savefig("trace.pdf")
	plt.show()
