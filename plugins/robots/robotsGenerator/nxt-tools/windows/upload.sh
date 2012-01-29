#! /bin/bash
# export $PATH="$PATH:/bin"
cd /example0/
make clean
make all
chmod u+x rxeflash.sh
sh ./rxeflash.sh
echo "Done"
