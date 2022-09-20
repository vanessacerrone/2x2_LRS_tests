# 2x2 ArgonCube LRS tests
Set of scripts for LED calibration run for final Light Readout System (LRS) QA/QC tests

![versions](https://img.shields.io/badge/ROOT-v6-blue?) ![versions](https://img.shields.io/pypi/pyversions/pybadges.svg)

**To automatically launch analysis for all channels:**
~~~bash
$ . run.sh DATAPATH/ROOT_FILE
~~~
Default options:
+ `n_channels = 64`
+ `maxpeaks = 11`
+ `verbose = 2`
  

**led_calibration.C** : extract gain/calibration factor by analyzing single photoelectron (p.e.)
spectrum for each SiPM.

- `single_channel(const string infilename, short chan, int maxpeaks, int verbose = 0)`: perform analysis for a single channel
  + `chan`: channel number
  + `maxpeaks`: maximum number of p.e. peaks to look for
  + `verbose`: verbose mode (see code for details)
  
- `all_channels(string infilename, int n_channels, int maxpeaks, int verbose = 0)`: perform analysis for all channels.
  + `n_channels`: total number of channels
  + `maxpeaks`: maximum number of p.e. peaks to look for
  + `verbose`: verbose mode (see code for details)


Usage example: 

```bash
$ root -l
.L led_calibration.C
single_channel("DATAPATH/ROOT_FILE", 10, 9, 2) 
all_channels("DATAPATH/ROOT_FILE", 15, 10, 2)
.q 
```

Output:
- A csv file with channel number, gain/offset and corresponding uncertainties is produced and saved 
- If `verbose == 2`, plots are saved [here](plots/).

**set_style.C** : contains style settings


**gains_plot.py** : read output csv files and plot SiPM gain as a function of channel number. 

```bash

Usage: python3 gains_plot.py [-h] -f FILE -s SAVE [-d DEBUG]

Options: 
	-h, --help              : show this help message and exit
	-f FILE, --file FILE    : Input filename (mandatory)
	-s SAVE, --save SAVE    : Save plot with all channels (mandatory)
	-d DEBUG, --debug DEBUG : Plot valid channels (gain>0) with  
                                  corresponding connections to LRS (optional)   
```

```bash
Example: python3 gain_plot.py -f ACL_0cd913fb_20220207_054800.csv -s True -d True
```