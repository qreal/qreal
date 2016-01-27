#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/tools
mkdir -p $PWD/../data/plugins/editors
mkdir -p $PWD/../data/help

cp -r  $BIN_DIR/plugins/editors/changelog.txt                 $PWD/../data/

cp -r  $BIN_DIR/plugins/editors/pics                          $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/index.html                    $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/interface.html                $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/lang.html                     $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/modelling.html                $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/verification.html             $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/style.css                     $PWD/../data/help/
cp -r  $BIN_DIR/plugins/editors/version.js                    $PWD/../data/help/
