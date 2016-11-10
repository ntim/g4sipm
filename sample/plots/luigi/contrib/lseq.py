#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np

def lseq(start, stop, step=0.2):
    start = np.log(start)
    stop = np.log(stop)
    times = np.arange(start, stop, step)
    for i in range(len(times)):
        times[i] = np.exp(times[i])
    return times