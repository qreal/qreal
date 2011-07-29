!#bin/bash

cd $qrealdir/qrgui/generated/example
export PATH="$PATH:$qrealdir/arm-elf-gcc-4.3.3/bin"
make clean
make all
./rxeflash.sh
