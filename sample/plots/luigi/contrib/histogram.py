#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
from scipy.stats import chi2, beta
from lseq import *
from ROOT import TProfile

import scipy.stats
def clopper_pearson(k, n, alpha=0.32):
    """
    http://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval
    alpha confidence intervals for a binomial distribution of k expected successes on n trials
    Clopper Pearson intervals are a conservative estimate.
    """
    lo = beta.ppf(alpha / 2, k, n - k + 1)
    hi = beta.ppf(1 - alpha / 2, k + 1, n - k)
    return lo, hi

def histogram(x, y, log=False, step=0.2, nbins=None, assym_errors=False, bins=None, returnNbins=False):
    '''
    Compute histogram with mean and stdev per bin.
    '''
    x_min = min(x)
    x_max = max(x)
    if log and x_min <= 0:
        print "min(x) <= 0, setting to 1"
        x_min = 1
    if nbins != None:
        if log:
            step = np.log((x_max - x_min)) / nbins
        else:
            step = (x_max - x_min) / nbins
    if log:
        bins = lseq(x_min, x_max, step)
    else:
        bins = np.arange(x_min, x_max, step)
    n, _ = np.ma.array(np.histogram(x, bins=bins))
    sy, _ = np.ma.array(np.histogram(x, bins=bins, weights=y))
    sy2, _ = np.ma.array(np.histogram(x, bins=bins, weights=y * y))
    mean = sy / n
    std = np.sqrt(sy2 / n - mean * mean)
    if assym_errors:
        # Assymmetric errors: correct the lower bound.
        std_up = std
        std_low = std_up.copy()
        # Error should not go negative.
        std_low[std_up >= mean] = mean[std_up >= mean] * (1.0 - 1e-5)
        # Compute confidence interval assuming the number of fired cells is poisson distributed.
        conf = np.asarray(chi2.interval(.68, 2. * mean)) / 2.0 - mean
        conf[0] *= -1.0
        # return bins, mean, [std_low, std_up]
        return bins, mean, conf
    if returnNbins:
        return bins, mean, std, n
    return bins, mean, std

def profile(dx, dy, name, bins, xlow=None, xup=None, log=False):
    if xlow == None and xup == None:
        prof = TProfile("prof-%s" % name, "prof-%s" % name, int(len(bins)), bins, "s")
    else:
        if not log:
            prof = TProfile("prof-%s" % name, "prof-%s" % name, int(bins), xlow, xup, "s")
        else:
            if xlow <= 0:
                xlow = 1
            step = np.log((xup - xlow)) / bins
            bins = lseq(xlow, xup, step)
            prof = TProfile("prof-%s" % name, "prof-%s" % name, int(len(bins)) - 1, bins, "s")
    for xi, yi in zip(dx, dy):
        prof.Fill(xi, yi)
    x = np.array([prof.GetBinCenter(i) for i in xrange(prof.GetNbinsX()) if prof.GetBinContent(i) > 0])
    y = np.array([prof.GetBinContent(i) for i in xrange(prof.GetNbinsX()) if prof.GetBinContent(i) > 0])
    yerr = np.array([prof.GetBinError(i) for i in xrange(prof.GetNbinsX()) if prof.GetBinContent(i) > 0])
    n = np.array([prof.GetBinEntries(i) for i in xrange(prof.GetNbinsX()) if prof.GetBinContent(i) > 0])
    return x, y, yerr, n
