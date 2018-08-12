#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/editors/robotsMetamodel.dll                   $PWD/../data/plugins/editors/
cp    $BIN_DIR/robots-interpreter-core.dll                           $PWD/../data
cp    $BIN_DIR/plugins/tools/robots-plugin.dll                       $PWD/../data/plugins/tools/
cp    $BIN_DIR/robots-2d-model.dll                                   $PWD/../data
cp    $BIN_DIR/Box2D.dll                                             $PWD/../data
cp    $BIN_DIR/robots-generator-base.dll                             $PWD/../data
cp    $BIN_DIR/robots-kit-base.dll                                   $PWD/../data
cp    $BIN_DIR/robots-utils.dll                                      $PWD/../data
cp    $BIN_DIR/qextserialport.dll                                    $PWD/../data


cp    $QT_DIR/bin/Qt5Network.dll			$PWD/../data
cp    $QT_DIR/bin/Qt5Multimedia.dll			$PWD/../data

