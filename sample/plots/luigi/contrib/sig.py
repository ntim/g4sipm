#!/usr/bin/env python
# -*- coding: utf-8 -*-
import math
import __builtin__
 
def num_digits(x):
    '''
    Get number of digits until first significant for float x
    '''
    if x == 0:
        return 0
    return int(math.floor(math.log10(x)))
 
def round(x, sig=1):
    '''
    Round float x to the significant digits sig.
    '''
    return __builtin__.round(x, sig - num_digits(x) - 1)
 
def round_std(x, std, sig=1):
    '''
    Round float x and uncertainty sigma to the significant digits sig of sigma.
    '''
    n = num_digits(std)
    if n == 0:
        n = num_digits(x)
    return __builtin__.round(x, sig - n - 1), __builtin__.round(std, sig - n - 1)

def format(x, sig=1, exp=3):
    '''
    Format float x with LaTeX code. 
    Exp defines the threshold at which scientific notation will be used.
    '''
    n = sig - num_digits(x) - 1
    x = round(x, sig)
    if abs(n) >= exp:
        x *= 10 ** n
        return ("$%d \cdot 10^{%d}$") % (x, -n)
    if n >= 0:
        return ("%." + str(abs(n)) + "f") % x
    return "%d" % x
 
def format_std(x, std, sig=1, exp=3):
    '''
    Format float x and uncertainty to the significant digits sig of std with LaTeX code. 
    Exp defines the threshold at which scientific notation will be used.
    '''
    n = sig - num_digits(std) - 1
    x, sigma = round_std(x, std, sig)
    if abs(n) >= exp:
        x *= 10 ** n
        std *= 10 ** n
        return ("$(%d \pm %.d) \cdot 10^{%d}$") % (x, std, -n)
    if n >= 0:
        return ("(%." + str(abs(n)) + "f $\pm$ %." + str(abs(n)) + "f)") % (x, sigma)
    return ("(%d $\pm$ %d)") % (x, std)
