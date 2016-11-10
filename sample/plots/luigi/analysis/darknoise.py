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

