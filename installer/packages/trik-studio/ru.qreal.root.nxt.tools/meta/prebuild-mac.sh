#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/common_utils.sh


cd "$(dirname "$0")"/../data/bin
rm -rf nxt-tools
