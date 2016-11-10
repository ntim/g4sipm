#!/usr/bin/env python
import argparse
import glob
import os
import matplotlib.pyplot as plt
import pandas as pd
import pickle
import numpy as np
import uncertainties.unumpy as unp

parser = argparse.ArgumentParser()
parser.add_argument("--path", help="the path to the luigi simulation results directory", default="./results/darknoise-simulation/hamamatsu-s10362-33-050c-niggemann/darknoise-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
df_exp = pd.read_pickle(os.path.join(os.path.dirname(args.path), "darknoise-simulation-expected.pkl"))

f, axes = plt.subplots(2, 2)
# f.set_figheight(f.get_figheight() * 2.0, forward=True)
f.subplots_adjust(left=0.1, right=0.97, wspace=0.3)

# Plot crosstalk.
ax = axes[0][1]
eb = ax.errorbar(df.v_ov, unp.nominal_values(df.p_ct_fit) * 100, yerr=unp.std_devs(df.p_ct_fit) * 100, fmt='.')
ax.plot(df_exp.v_ov, df_exp.p_ct.values * 100.0, color=eb.lines[0].get_color(), linestyle=":")
ax.set_ylabel("crosstalk $ p_{ct}$ / \\%")

# Plot afterpulsing
ax = axes[1][0]
eb1 = ax.errorbar(df.v_ov, unp.nominal_values(df.p_ap_f) * 100, yerr=unp.std_devs(df.p_ap_f) * 100, fmt='.', label="$ p_{ap,f}$")
eb2 = ax.errorbar(df.v_ov, unp.nominal_values(df.p_ap_s) * 100, yerr=unp.std_devs(df.p_ap_s) * 100, fmt='.', label="$ p_{ap,s}$")
ax.plot(df_exp.v_ov, df_exp.p_ap_s.values * 100.0, color=eb1.lines[0].get_color(), linestyle=":")
ax.plot(df_exp.v_ov, df_exp.p_ap_f.values * 100.0, color=eb2.lines[0].get_color(), linestyle=":")
p_ap = 1. - (1. - df.p_ap_f) * (1. - df.p_ap_s)
p_ap_exp = 1. - (1. - df_exp.p_ap_f) * (1. - df_exp.p_ap_s)
eb3 = ax.errorbar(df.v_ov, unp.nominal_values(p_ap) * 100, yerr=unp.std_devs(p_ap) * 100, fmt='.', label="$ p_{ap}$")
ax.plot(df_exp.v_ov, p_ap_exp.values * 100.0, color=eb3.lines[0].get_color(), linestyle=":")
ax.set_ylabel("afterpulsing $ p_{ap}$ / \\%")
ax.legend(loc="upper left")

# Afterpulsing time constants.
ax = axes[1][1]
apf = ax.errorbar(df.v_ov, unp.nominal_values(df.tau_ap_f), yerr=unp.std_devs(df.tau_ap_f), fmt='.', label="$\\tau_{ap,f}$")
aps = ax.errorbar(df.v_ov, unp.nominal_values(df.tau_ap_s), yerr=unp.std_devs(df.tau_ap_s), fmt='.', label="$\\tau_{ap,s}$")
ax.plot(df_exp.v_ov, df_exp.tau_ap_f.values, color=apf.lines[0].get_color(), linestyle=":")
ax.plot(df_exp.v_ov, df_exp.tau_ap_s.values, color=aps.lines[0].get_color(), linestyle=":")
ax.set_ylabel("afterpulsing $\\tau_{ap}$ / ns")
ax.legend(loc="upper right")

# Thermal noise.
ax = axes[0][0]
f_th = 1 / df.tau_th * 1e3  
eb = ax.errorbar(df.v_ov, unp.nominal_values(f_th), yerr=unp.std_devs(f_th), fmt='.')
ax.set_ylabel("thermal noise $ f_{th}$ / MHz")
ax.plot(df_exp.v_ov, df_exp.f_th.values * 1e3, color=eb.lines[0].get_color(), linestyle=":")

#
# Plot residuals
#
def residual(meas, exp):
    return (meas - exp) / exp * 100.0
    
f, axes = plt.subplots(2, 2)
# f.set_figheight(f.get_figheight() * 2.0, forward=True)
f.subplots_adjust(left=0.1, right=0.97, wspace=0.3)

# Plot crosstalk.
ax = axes[0][1]
eb = ax.errorbar(df.v_ov, unp.nominal_values(residual(df.p_ct_fit, df_exp.p_ct.values)), yerr=unp.std_devs(residual(df.p_ct_fit, df_exp.p_ct.values)), fmt='.')
ax.set_ylabel("crosstalk $ p_{ct}$ / \\%")

# Plot afterpulsing
ax = axes[1][0]
eb1 = ax.errorbar(df.v_ov, unp.nominal_values(residual(df.p_ap_f, df_exp.p_ap_f.values)), yerr=unp.std_devs(residual(df.p_ap_f, df_exp.p_ap_f.values)), fmt='.', label="$ p_{ap,f}$")
eb2 = ax.errorbar(df.v_ov, unp.nominal_values(residual(df.p_ap_s, df_exp.p_ap_s.values)), yerr=unp.std_devs(residual(df.p_ap_s, df_exp.p_ap_s.values)), fmt='.', label="$ p_{ap,s}$")
p_ap = 1. - (1. - df.p_ap_f) * (1. - df.p_ap_s)
p_ap_exp = 1. - (1. - df_exp.p_ap_f) * (1. - df_exp.p_ap_s)
eb3 = ax.errorbar(df.v_ov, unp.nominal_values(residual(p_ap, p_ap_exp)), yerr=unp.std_devs(residual(p_ap, p_ap_exp)), fmt='.', label="$ p_{ap}$")
ax.set_ylabel("afterpulsing $ p_{ap}$ / \\%")
ax.legend(loc="upper left")

# Afterpulsing time constants.
ax = axes[1][1]
aps = ax.errorbar(df.v_ov, unp.nominal_values(residual(df.tau_ap_f, df_exp.tau_ap_f.values)), yerr=unp.std_devs(residual(df.tau_ap_f, df_exp.tau_ap_f.values)), fmt='.', label="$\\tau_{ap,f}$")
apf = ax.errorbar(df.v_ov, unp.nominal_values(residual(df.tau_ap_s, df_exp.tau_ap_s.values)), yerr=unp.std_devs(residual(df.tau_ap_s, df_exp.tau_ap_s.values)), fmt='.', label="$\\tau_{ap,s}$")
ax.set_ylabel("afterpulsing $\\tau_{ap}$ / ns")
ax.legend(loc="upper right")
   
# Thermal noise.
ax = axes[0][0]
f_th = 1 / df.tau_th * 1e3  
eb = ax.errorbar(df.v_ov, unp.nominal_values(residual(f_th, df_exp.f_th.values * 1e3)), yerr=unp.std_devs(residual(f_th, df_exp.f_th.values * 1e3)), fmt='.')
ax.set_ylabel("thermal noise $ f_{th}$ / MHz")
 
plt.show()

