# 2x2 ArgonCube LRS tests
Set of scripts for LED calibration run for final Light Readout System (LRS) QA/QC tests

### Prerequisites

![versions](https://img.shields.io/badge/ROOT-v6-blue)

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
- root -l
- .L led_calibration.C
- single_channel("filename.root", 10, 9, 2) 
- all_channels("filename.root", 15, 10, 2)
- .q 

Output:
- If `verbose == 2`, plots are saved in plots/\<ch>fit.pdf and plots/\<ch>spectrum.pdf

**set_style.C** : contains style settings



