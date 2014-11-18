#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-interpreter.dll    $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-qts-generator.dll  $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/robots-trik-runtime-uploader-plugin.dll                 $PWD/../data/
cp    $BIN_DIR/robots-trik-generator-base.dll                          $PWD/../data/


cp    $QT_DIR/bin/Qt5Network.dll                                       $PWD/../data
