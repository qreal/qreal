#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/
cp -r  $BIN_DIR/nxt-tools  $PWD/../data/
