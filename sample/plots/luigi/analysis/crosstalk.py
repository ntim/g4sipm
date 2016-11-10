#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
from ROOT import TH1, Double

def cell_id_to_pos(id, ncells):
    j = id % np.sqrt(ncells)
    i = (id - j) / np.sqrt(ncells)
    return i, j

def num_neighbours(id, ncells, shells):
    i, j = cell_id_to_pos(id, ncells)
    # Shells == 0 corresponds to 4 neighbour model.
    if shells == 0:
        i_min = np.maximum(0, i - 1)
        i_max = np.minimum(i + 1 + 1, np.sqrt(ncells))
        j_min = np.maximum(0, j - 1)
        j_max = np.minimum(j + 1 + 1, np.sqrt(ncells))
        return (i_max - i_min) + (j_max - j_min) - 2
    i_min = np.maximum(0, i - shells)
    i_max = np.minimum(i + shells + 1, np.sqrt(ncells))
    j_min = np.maximum(0, j - shells)
    j_max = np.minimum(j + shells + 1, np.sqrt(ncells))
    return (i_max - i_min) * (j_max - j_min) - 1

def edge_correction_factor(ncells, neighbours):
    shells = (np.sqrt(neighbours + 1) - 1) / 2
    max_neighbours = 4 * shells * (shells + 1)
    if neighbours == 4:
        shells = 0
        max_neighbours = 4
    nom = np.sum(num_neighbours(np.arange(ncells), ncells, shells))
    denom = 1.0 * max_neighbours * ncells
    return nom / denom

def prob(h, ncells, neighbours=4):
    # Compute integral and error
    n_err = Double()
    n = h.IntegralAndError(h.GetXaxis().GetFirst(), h.GetXaxis().GetLast(), n_err)
    # Compute crosstalk integral and error
    nct_err = Double()
    up, low = h.FindBin(0.6), h.FindBin(1.4)
    nct = h.IntegralAndError(up, low, nct_err)
    # Calculate probability.
    e = edge_correction_factor(ncells, neighbours)
    p = (n - nct) / n / e
    std = np.sqrt(((1 / n / e - p / n) * n_err) ** 2 + (nct_err / n / e) ** 2)
    #
    return p, std
