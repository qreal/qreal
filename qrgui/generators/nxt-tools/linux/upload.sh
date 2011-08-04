#!/bin/bash


cd $QREALDIR/example0
export PATH="$PATH:$QREALDIR/nxt-tools/gnuarm/bin"
make clean
make all
chmod +x ./rxeflash.sh
./rxeflash.sh
