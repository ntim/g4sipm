#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Plots the distribution of the cell triggers into a 2d-histogram.
# 

import sys, os, glob
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import persistency

def get_digi_cell_position(model, digi):
    """
    Determines the cell position.
    @param model - the SiPM model.
    @param digi - the digi.
    """
    import numpy as np
    sqrtN = np.sqrt(model['numberOfCells']);
    cellId = digi['cellId']
    j = (cellId % sqrtN);
    i = (cellId - j) / sqrtN;
    xC = (i + .5) * model['cellPitch'] - model['pitch'] / 2.;
    yC = (j + .5) * model['cellPitch'] - model['pitch'] / 2.;
    return xC, yC

if __name__ == "__main__":
    filename = glob.glob('results/*.txt')[-1]
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    filter = None
    if len(sys.argv) > 2:
        filter = int(sys.argv[2])
        
    fp = open(filename)
    model = persistency.get(fp, "G4SipmModel")
    digis = persistency.get_multiple(fp, "G4SipmDigi")
    
    x, y = [], []
    for d in digis:
        if not filter == None:
            if d['type'] != filter:
                continue
        xi, yi = get_digi_cell_position(model, d)
        x.append(xi)
        y.append(yi)
        
    bins = [i for i in np.arange(-model['pitch'] / 2., model['pitch'] / 2. + model['cellPitch'], model['cellPitch'])]
    
    H, xedges, yedges = np.histogram2d(x, y, bins=(bins, bins))
    extent = [yedges[0], yedges[-1], xedges[0], xedges[-1]]
    
    plt.imshow(H, extent=extent, interpolation='nearest', aspect="equal", cmap = cm.Reds)
    cb = plt.colorbar()
    cb.set_label("entries")
    plt.xlabel('x / mm')
    plt.ylabel('y / mm')
    plt.show()
