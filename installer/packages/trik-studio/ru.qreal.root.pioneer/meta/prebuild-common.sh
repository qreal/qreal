#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/editors
mkdir -p $PWD/../data/plugins/tools/kitPlugins

cp $BIN_DIR/pioneerStart.sh	$PWD/../data/
cp $BIN_DIR/pioneerUpload.sh	$PWD/../data/
cp $BIN_DIR/controller.py	$PWD/../data/
