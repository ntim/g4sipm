'''
// Single PE spectrum, fit function, according to FACT performance paper
// Note that the function as written doen here is only valid up to N=50
Double_t fcn_g(Double_t *xx, Double_t *par)
{
    const Double_t ampl  = par[0];      // amplitude of single pe peak
    const Double_t gain  = par[1];      // gain
    const Double_t sigma = par[2]*gain; // rel width of single pe peak
    const Double_t cross = par[3];      // crosstalk (note, this is not p>=2)
    const Double_t shift = par[4];      // baseline
    const Double_t noise = par[5]<0 ? sigma : par[5]; // electronics noise
    const Double_t expo  = par[6];      // Erlang exponent

    const Double_t P = cross*TMath::Exp(-cross);

    Double_t y = 0;

    // For speed reasons only 50 pe are considered... to go beyond
    // 50 pe, this limit needs to be increased
    for (int N=1; N<50; N++)
    {
        const Double_t muN  = N*gain + shift;
        const Double_t sigN = TMath::Sqrt(N*sigma*sigma + noise*noise);

        const Double_t p =
            TMath::Power(N*P, N-1)/TMath::Power(TMath::Factorial(N-1), expo);

        // Smear each N with a gauss according to single pe width,
        // electronics noise, gain and baseline
        y += TMath::Gaus(xx[0], muN, sigN) * p / sigN;
    }

    const Double_t sig1 = TMath::Sqrt(sigma*sigma + noise*noise);
    return ampl*sig1*y;
}
'''

import numpy as np
import crosstalk
from ROOT import TH1, Double, ROOT, TF1
from scipy.misc import factorial

n_max = 12

def gaus(x, mean, sigma):
    if sigma == 0:
        return 1.0e30
    arg = (x - mean) / sigma
    if arg < -39 or arg > 39.0:
        return 0.0
    return np.exp(-0.5 * arg * arg)

def _pn(n, p, expo):
    return np.power(n * p, n - 1) / np.power(factorial(n - 1), expo)

def _sigma_pn(n, p, expo, sigma_p, sigma_expo):
    pn = _pn(n, p, expo)
    return np.sqrt(np.power((n - 1) / p * pn * sigma_p, 2) + np.power(-np.log(factorial(n - 1)) * pn * sigma_expo, 2))

def single_pe_spectrum(x, par):
    ampl = par[0]  # amplitude of single pe peak
    gain = par[1]  # gain
    sigma = par[2] * gain  # rel width of single pe peak
    cross = par[3]  # crosstalk (note, this is not p>=2)
    shift = par[4]  # baseline
    noise = sigma if par[5] < 0 else par[5]  # electronics noise
    expo = par[6]  # Erlang exponent
    p = cross * np.exp(-cross)
    y = 0
    for n in xrange(1, n_max):
        muN = n * gain + shift
        sigN = np.sqrt(n * sigma * sigma + noise * noise)
        # Smear each N with a gauss according to single pe width,
        # electronics noise, gain and baseline
        y += gaus(x[0], muN, sigN) * _pn(n, p, expo) / sigN
    sig1 = np.sqrt(sigma * sigma + noise * noise)
    return ampl * sig1 * y

def compute_crosstalk_probability(par, par_errors):
    ampl = par[0]  # amplitude of single pe peak
    gain = par[1]  # gain
    sigma = par[2] * gain  # rel width of single pe peak
    cross = par[3]  # crosstalk (note, this is not p>=2)
    sigma_cross = par_errors[3]
    shift = par[4]  # baseline
    noise = sigma if par[5] < 0 else par[5]  # electronics noise
    expo = par[6]  # Erlang exponent
    sigma_expo = par_errors[6]
    p = cross * np.exp(-cross)
    sigma_p = (np.exp(-cross) - p) * sigma_cross
    nom = 0
    sigma_nom2 = 0
    denom = 0
    sigma_denom2 = 0
    for n in xrange(1, n_max):
        pn = _pn(n, p, expo)
        sigma_pn = _sigma_pn(n, p, expo, sigma_p, sigma_expo)
        denom += pn
        sigma_denom2 += sigma_pn * sigma_pn
        if n > 1:
            nom += pn
            sigma_nom2 += sigma_pn * sigma_pn
    p_ct = nom / denom
    std = np.sqrt(sigma_denom2 / (denom * denom) + sigma_nom2 / (nom * nom))
    return p_ct, std

from scipy.misc import factorial
def erlang(x, par):
    a = par[0]
    k = par[1]
    l = par[2]
    return a * np.power(l, k) * np.power(x[0], k - 1) * np.exp(-l * x[0]) / factorial(k - 1)

def mod_erlang(x, par):
    a = par[0]
    k = par[1]
    l = par[2]
    nu = par[3]
    return a * np.power(l, k) * np.power(x[0], k - 1) * np.exp(-l * x[0]) / np.power(factorial(k - 1), nu)


def fit(h, ncells, neighbours, fix_gain=False, fix_sigma_el=False, fix_mu_0=False):
    # Set default fitter.
    ROOT.Math.MinimizerOptions.SetDefaultTolerance(1e-3)
    ROOT.Math.MinimizerOptions.SetDefaultMinimizer("Minuit2")
    ROOT.Math.MinimizerOptions.SetDefaultMaxIterations(1000)
    ROOT.Math.MinimizerOptions.SetDefaultMaxFunctionCalls(1000)
    ROOT.Math.MinimizerOptions.SetDefaultPrecision(1e-9)
    # Create fit fittion
    fit = TF1("fitPe", single_pe_spectrum, h.GetBinLowEdge(1), h.GetBinLowEdge(h.GetNbinsX()), 7);
    fit.SetParNames("A", "G", "#sigma_{pe}", "p_{ct}", "#mu_{0}", "#sigma_{el}", "#nu")
    fit.SetParameter(0, h.GetMaximum())  # peak
    fit.SetParLimits(0, 0.5 * h.GetMaximum(), 1.5 * h.GetMaximum())
    fit.SetParameter(1, 1.0)  # gain
    if fix_gain:
        fit.FixParameter(1, 1.0)
    else:
        fit.SetParLimits(1, 0.8, 1.2)
    fit.SetParameter(2, 0.1)  # sigma
    fit.SetParLimits(2, 0.01, 0.3)
    fit.SetParameter(3, 0.1)  # crosstalk
    fit.SetParLimits(3, 0.01, 0.6)
    fit.SetParameter(4, 0)  # shift
    if fix_mu_0:
        fit.FixParameter(4, 0.0)
    else:
        fit.SetParLimits(4, -1, 1)
    fit.SetParameter(5, 0.01)  # noise
    if fix_sigma_el:
        fit.FixParameter(5, 0.0)
    else:
        fit.SetParLimits(5, 0.01, 0.6)
    fit.SetParameter(6, 1.0)  # expo
    fit.SetParLimits(6, 0.6, 1.4)
    h.Fit(fit, "RNM")
    h.GetListOfFunctions().Clear()
    h.GetListOfFunctions().Add(fit)
    # Compute real crosstalk probability
    p_ct_fit, p_ct_fit_err = compute_crosstalk_probability(fit.GetParameters(), fit.GetParErrors())
    edge_correction_factor = crosstalk.edge_correction_factor(ncells, neighbours)
    p_ct_fit /= edge_correction_factor
    p_ct_fit_err /= edge_correction_factor
    # Get parameters.
    amp = fit.GetParameter(0)
    amp_err = fit.GetParError(0)
    g = fit.GetParameter(1)
    g_err = fit.GetParError(1)
    sigma_pe = fit.GetParameter(2)
    sigma_pe_err = fit.GetParError(2)
    p_ct = fit.GetParameter(3)
    p_ct_err = fit.GetParError(3)
    mu_0 = fit.GetParameter(4)
    mu_0_err = fit.GetParError(4)
    sigma_el = fit.GetParameter(5)
    sigma_el_err = fit.GetParError(5)
    nu = fit.GetParameter(6)
    nu_err = fit.GetParError(6)
    #
    return p_ct_fit, p_ct_fit_err, amp, amp_err, g, g_err, sigma_pe, sigma_pe_err, p_ct, p_ct_err, mu_0, mu_0_err, sigma_el, sigma_el_err, nu, nu_err, fit.GetChisquare(), fit.GetNDF()
