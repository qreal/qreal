#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/tools
mkdir -p $PWD/../data/plugins/editors
mkdir -p $PWD/../data/help

cp -r  $BIN_DIR/plugins/editors/changelog.txt                 $PWD/../data/
