#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/common_utils.sh
cd $PWD/../data/bin
mv nxt-tools tmp
mv tmp/linux/nxt-tools -t .
rm -rf tmp
