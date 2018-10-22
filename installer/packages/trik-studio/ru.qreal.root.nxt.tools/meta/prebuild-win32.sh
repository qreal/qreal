#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/common_utils.sh

cd $PWD/../data

mv bin/nxt-tools/win/nxt-tools -t .
dos2unix nxt-tools/compile.sh
rm -rf bin
