#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

rm -rf $PWD/../data
mkdir -p $PWD/../data/bin
source $INSTALLER_ROOT/utils/common_utils.sh

cp     $INSTALLER_ROOT/licenses/WinScp-license.txt                        $PWD
