#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#
import numpy as np

def rs(n1, n2, theta):
	nom = n1 * np.cos(theta) - n2 * np.sqrt(1 - (n1 / n2 * np.sin(theta)) ** 2)
	denom = n1 * np.cos(theta) + n2 * np.sqrt(1 - (n1 / n2 * np.sin(theta)) ** 2)
	return np.abs(nom / denom) ** 2.

def rp(n1, n2, theta):
	nom = n1 * np.sqrt(1 - (n1 / n2 * np.sin(theta)) ** 2) - n2 * np.cos(theta)
	denom = n1 * np.sqrt(1 - (n1 / n2 * np.sin(theta)) ** 2) + n2 * np.cos(theta)
	return np.abs(nom / denom) ** 2.

def r(n1, n2, theta):
	return (rs(n1, n2, theta) + rp(n1, n2, theta)) / 2.

def t(n1, n2, theta):
	return 1. - r(n1, n2, theta)

def snell(n1, n2, theta):
	"""
	@param n1
	@param n2
	@param theta - the incidence angle
	@return the refraction angle.
	"""
	return np.arcsin(n1 / n2 * np.sin(theta))

def multi(n1, n2, n3, theta):
	"""
	Derives the multi layer transmission probability for through 2 optical borders.
	@param n1
	@param n2
	@param n3
	@param theta
	@return the transmission	
	"""
	eta = snell(n1, n2, theta)
	nom = t(n1, n2, theta) * t(n2, n3, eta)
	denom = (1. - r(n2, n1, eta) * r(n2, n3, eta))
	return (nom / denom)
