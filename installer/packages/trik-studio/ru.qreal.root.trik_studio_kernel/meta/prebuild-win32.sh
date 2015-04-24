#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/editors/robotsMetamodel.dll                   $PWD/../data/plugins/editors/
cp    $BIN_DIR/robots-interpreter-core.dll                           $PWD/../data
cp    $BIN_DIR/plugins/tools/robots-plugin.dll                       $PWD/../data/plugins/tools/
cp    $BIN_DIR/robots-2d-model.dll                                   $PWD/../data
cp    $BIN_DIR/robots-generator-base.dll                             $PWD/../data
cp    $BIN_DIR/robots-kit-base.dll                                   $PWD/../data
cp    $BIN_DIR/robots-utils.dll                                      $PWD/../data
