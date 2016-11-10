#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, glob
import matplotlib.pyplot as plt
import numpy as np
import sqlite3

millivolt = 1e-9
		
if __name__ == "__main__":
	filename = glob.glob('results/*.sqlite')[-1]
	if len(sys.argv) > 1:
		filename = sys.argv[1]
	# Plot range from arguments
	start = 0
	stop = 1000
	if len(sys.argv) > 3:
		start = int(sys.argv[2])
		stop = start + int(sys.argv[3])
	# Open file.
	con = sqlite3.connect(filename)
	cur = con.cursor()
	# Extract trace.
	voltages = []
	times = []
	for row in cur.execute("SELECT time, voltage FROM `g4sipmVoltageTraceDigis-0` WHERE time BETWEEN %g AND %g;" % (start, stop)):
		voltages.append(row[1] / millivolt)
		times.append(row[0] / 1000)
	# Plot traces superposed.
	plt.plot(times, voltages, '-')
	#
	name, pitch, cellPitch, numberOfCells = cur.execute("SELECT name, pitch, cellPitch, numberOfCells FROM sipmModel;").fetchone()
	plt.text(0.025, 0.975, "%s\n%d x %d mm, %d $\mu$m pitch\n%d cells"  % (name, pitch, pitch, cellPitch * 1000, numberOfCells), 
			ha="left", va="top", fontsize="medium", transform=plt.axes().transAxes)
	plt.ylabel('voltage / mV')
	plt.xlabel(u'time / µs')
	plt.savefig("trace.pdf")
	plt.show()
