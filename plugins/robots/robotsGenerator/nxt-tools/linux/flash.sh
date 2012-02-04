#!/bin/bash

cd $QREALDIR/nxt-tools/libnxt
modprobe -r sam_ba cdc_acm
./fwflash lms_arm_nbcnxc_107.rfw
