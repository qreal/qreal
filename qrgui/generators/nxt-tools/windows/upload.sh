#!/bin/bash

export PATH="$PATH:$qrealdir/cygwin/bin"
export PATH="$PATH:$qrealdir/cygwin/GNUARM/bin"
export PATH="$PATH:$qrealdir/nexttool"
cd $qrealdir/qrgui/generators/example
make clean
make all
sh ./rxeflash.sh
echo "Done"