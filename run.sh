#!/bin/bash

root -b -l << EOF

.L led_calibration.C
all_channels("$1",15,11,2)
.q
EOF
