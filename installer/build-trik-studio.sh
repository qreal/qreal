#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


# Usage: build-trik-studio.sh <path-to-qt>. See build-installer.sh for more info.


chmod +x $PWD/build-installer.sh && $PWD/build-installer.sh $1 trik-studio
