#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
from ROOT import TH1, Double, ROOT, TF1

def prob_dist(x, par):
    dt = x[0]
    amp = par[0]
    pS = par[1]
    pL = par[2]
    tauS = par[3]
    tauL = par[4]
    tauTh = par[5]
    T = np.exp(-dt / tauTh)
    S = np.exp(-dt / tauS)
    L = np.exp(-dt / tauL)
    pTot = 0
    pTot += T * (1. - pS) * (1. - pL) / tauTh
    pTot += T * S * pS * (1. - pL) * (1. / tauS + 1. / tauTh)
    pTot += T * L * pL * (1. - pS) * (1. / tauL + 1. / tauTh)
    pTot += T * S * L * pS * pL * (1. / tauS + 1. / tauL + 1. / tauTh)
    return amp * pTot

def fit(h, xlow=0):
    # Set default fitter.
    ROOT.Math.MinimizerOptions.SetDefaultMinimizer("Minuit2", "Migrad")
    # Fit thermal noise component.
    preFit = TF1("preFit", "[0]*exp(-x/[1])", 600, h.GetBinLowEdge(h.GetNbinsX()))
    preFit.SetParameter(1, 1000)
    preFit.SetParLimits(1, 10, 10000)  # 100kHz to 10MHz
    h.Fit(preFit, "RN")
    # Fit complete distribution.
    fit = TF1("fitDeltaT", prob_dist, xlow, h.GetBinLowEdge(h.GetNbinsX()), 6)
    fit.SetParNames("A", "P_{s}", "P_{l}", "#tau_{s}", "#tau_{l}", "#tau_{th}")
    fit.SetParameters(1., 0.2, 0.2, 50, 100, preFit.GetParameter(1))
    fit.SetParLimits(1, 0.001, 1.)
    fit.SetParLimits(2, 0.001, 1.)
    fit.SetParLimits(3, 5., 80.)
    fit.SetParLimits(4, 80., 240.)
    fit.SetParLimits(5, preFit.GetParameter(1) - 3. * preFit.GetParError(1),
    preFit.GetParameter(1) + 3. * preFit.GetParError(1))
    h.Fit(fit, "RNM")
    h.GetListOfFunctions().Add(fit)
    # Return results
    amp = fit.GetParameter(0)
    amp_err = fit.GetParError(0)
    p_ap_s = fit.GetParameter(1)
    p_ap_s_err = fit.GetParError(1)
    p_ap_l = fit.GetParameter(2)
    p_ap_l_err = fit.GetParError(2)
    tau_s = fit.GetParameter(3)
    tau_s_err = fit.GetParError(3)
    tau_l = fit.GetParameter(4)
    tau_l_err = fit.GetParError(4)
    tau_th = fit.GetParameter(5)
    tau_th_err = fit.GetParError(5)
    return amp, amp_err, p_ap_s, p_ap_s_err, p_ap_l, p_ap_l_err, tau_s, tau_s_err, tau_l, tau_l_err, tau_th, tau_th_err, fit.GetChisquare(), fit.GetNDF()
