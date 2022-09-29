#!/bin/bash

# run led calibration script
# usage: . run.sh filename 

echo "Analysing file: $1"

root -b -l << EOF

.L led_calibration.C
all_channels("$1",64,11,2)
.q
EOF
