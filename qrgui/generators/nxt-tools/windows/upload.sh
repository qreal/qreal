#!/bin/bash

export PATH="$PATH:$qrealdir/nxt-tools/cygwin/bin"
export PATH="$PATH:$qrealdir/nxt-tools/cygwin/GNUARM/bin"
export PATH="$PATH:$qrealdir/nxt-tools/nexttool"
cd $qrealdir/example0
make clean
make all
sh ./rxeflash.sh
echo "Done"