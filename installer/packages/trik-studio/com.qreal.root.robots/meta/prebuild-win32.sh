#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/editors/robotsMetamodel.dll                   $PWD/../data/plugins/editors/
cp    $BIN_DIR/plugins/tools/interpreterCore.dll                     $PWD/../data/plugins/tools/
cp    $BIN_DIR/commonTwoDModel.dll                                   $PWD/../data
cp    $BIN_DIR/generatorBase.dll                                     $PWD/../data
cp    $BIN_DIR/interpreterBase.dll                                   $PWD/../data
