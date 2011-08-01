!#bin/bash

cd $qrealdir/libnxt
modprobe -r sam_ba
./fwflash lms_arm_nbcnxc_107.rfw
