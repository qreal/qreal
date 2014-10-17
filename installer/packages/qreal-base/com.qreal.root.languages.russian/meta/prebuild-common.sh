#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/translations
cp -r $BIN_DIR/translations/ru $PWD/../data/translations
