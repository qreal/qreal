#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/tools
mkdir -p $PWD/../data/plugins/editors
mkdir -p $PWD/../data/help
mkdir -p $PWD/../data/sqldrivers/

cp -r  $BIN_DIR/changelog.txt                      $PWD/../data/

cp -r  $BIN_DIR/help/pics                          $PWD/../data/help/
cp -r  $BIN_DIR/help/index.html                    $PWD/../data/help/
cp -r  $BIN_DIR/help/interface.html                $PWD/../data/help/
cp -r  $BIN_DIR/help/lang.html                     $PWD/../data/help/
cp -r  $BIN_DIR/help/modelling.html                $PWD/../data/help/
cp -r  $BIN_DIR/help/operations.html               $PWD/../data/help/
cp -r  $BIN_DIR/help/style.css                     $PWD/../data/help/
cp -r  $BIN_DIR/help/version.js                    $PWD/../data/help/

cp -r  $BIN_DIR/translations/ru		               $PWD/../data/translations