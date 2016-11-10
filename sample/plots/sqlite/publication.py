#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib as mpl
import numpy as np

def apply(fig_width_pt=224.0):
    factor = 2
    inches_per_pt = 1.0 / 72.27 # Convert pt to inches
    golden_mean = (np.sqrt(5) - 1.0) / 2.0  # Aesthetic ratio
    fig_width = fig_width_pt * inches_per_pt  # width in inches
    fig_height = fig_width * golden_mean  # height in inches
    fig_size = [fig_width * factor, fig_height * factor]
    mpl.rcdefaults()
    size = 7.5 * factor
    params = { 
            'font.family': 'Century Schoolbook L,Nimbus Roman No9 L',
            'font.size' : size,
            'axes.labelsize' : size,
            'font.size' : size,
            'text.fontsize' : size,
            'legend.fontsize': size,
            'xtick.labelsize' : size,
            'ytick.labelsize' : size,
            'text.usetex': True,
            'axes.labelweight': 'medium',
            'axes.color_cycle': 'k',
            'legend.frameon': False,
            'legend.numpoints': 3,
            'figure.figsize': fig_size }
    mpl.rcParams.update(params)
    # mpl.rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
