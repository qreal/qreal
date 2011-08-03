#!/bin/bash

cd $QREALDIR/qrgui/generated/example
export PATH="$PATH:$QREALDIR/arm-elf-gcc-4.3.3/bin"
make clean
make all
./rxeflash.sh
