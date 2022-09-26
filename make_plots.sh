#!/bin/bash

# usage: . make_plots.sh filename

echo "Analysing file: $1"

python3 gains_plot.py -f $1 -s True -d True

