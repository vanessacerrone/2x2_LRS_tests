# -----------------------------------------------------------
# Created : September 2022
# Author  : Vanessa Cerrone
# Usage   : python3 gain_plot.py -f <filename>
#
# Open output csv file from led_calibration.C
# Save data into a pandas dataframe
# Plot gain as a function of channel number
# -----------------------------------------------------------



# system information
import os
import argparse

# computing
import numpy as np
import pandas as pd
import scipy as sp
import scipy.optimize as op

# plotting
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib import gridspec
from matplotlib.ticker import AutoMinorLocator, MultipleLocator


# define ticks
def set_ticks(ax, xMaj, yMaj):
    ax.xaxis.set_major_locator(MultipleLocator(xMaj))
    ax.yaxis.set_major_locator(MultipleLocator(yMaj))
    ax.minorticks_on()
    ax.tick_params(which='major', width=1.0, length=8, direction='in', labelsize=14)
    ax.tick_params(which='minor', width=1.0, length=4, direction='in', labelsize=14)
    ax.yaxis.get_offset_text().set_fontsize(14)
    ax.xaxis.get_offset_text().set_fontsize(14)



def read_data(infile):
    '''
        Read csv files and convert into dataframe (df)
        *Input:     csv file 
                    columns: Channel,Peaks,Gain,Error_g, Offset,Error_o

        *Output:    original df, valid channels df, only pedestal df, inactive ch df 

    '''
    df = pd.read_csv(infile, sep=',')
    df_valid = df[df['Gain'] > 0]
    df_ped = df[df['Gain'] < 0]
    df_null = df[df['Gain'] == 0]

    return df, df_valid, df_ped, df_null



def plot(infile, save = True):

    df, df_valid, df_ped, df_null = read_data(infile)

    s = infile.split('_')
    number = s[-1].split('.')
    outfile = s[0] + '_' + s[2] + '_' + number[0]


    fig, ax = plt.subplots(figsize=(8,6))

    ax.plot(df_valid['Channel'], df_valid['Gain'], marker='o', mec='white', markersize=7,
                    color='#004C97', linestyle='none', label='Valid channels')

    ax.plot(df_ped['Channel'], df_ped['Gain'], marker='s', mec='white', markersize=6,
                    color='#4C8C2B', linestyle='none', label='Only pedestal')

    ax.plot(df_null['Channel'], df_null['Gain'], marker='d', mec='white', markersize=7,
                    color='#AF272F', linestyle='none', label='Inactive channels')

    # labels 
    ax.set_ylabel('SiPM Gain [ADC / p.e.]', fontsize = 14)
    ax.set_xlabel('Channel number', fontsize = 14)

    # ticks
    set_ticks(ax, 5,500)
    ax.set_xlim(-2,67)

    # inset 
    ins = ax.inset_axes([0.5, 0.5, 0.45, 0.35], transform=ax.transAxes)
    

    ins.plot(df_valid['Channel'], df_valid['Gain'],color='#004C97', ls='-', alpha=0.4, lw=2)
    ins.plot(df_valid['Channel'], df_valid['Gain'], lw=0, marker='o',  alpha=1, markersize=3, color='#004C97')

    ins.text(0.8,0.8, 'Valid channels', fontsize = 8, transform=ax.transAxes )
    ins.grid(axis='y',ls='-',alpha=0.25, color='gray')

    ins.tick_params(which='major', width=1.0, length=8, direction='in', labelsize=10)
    ins.tick_params(which='minor', width=1.0, length=4, direction='in', labelsize=10)
    ins.minorticks_on()

    ax.indicate_inset_zoom(ins, edgecolor="gray")
    ax.legend(loc = 'best', prop = {'size': 11}, ncol = 1, frameon = False)

    plt.show()


    if save:
        fig.savefig(outfile + '.pdf', dpi = 200)





def main(infile):

    plot(infile, True)




def parse_args():
    '''Parse the args.'''
    parser = argparse.ArgumentParser()

    parser.add_argument('-f', '--file', type = str, required = True,
                        help = 'Input file',dest='file')

    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()
    main(infile = args.file)
