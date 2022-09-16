#!/bin/bash

# run led calibration script
# usage: .run.sh filename LRS
# where LRS is either LCM or ACL

root -b -l << EOF

.L led_calibration.C
all_channels("$1",64,11,2,"$2")
.q
EOF
