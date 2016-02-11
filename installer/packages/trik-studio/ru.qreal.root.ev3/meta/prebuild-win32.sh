#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/plugins/tools/kitPlugins/robots-ev3-interpreter.dll   $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-ev3-rbf-generator.dll $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/robots-ev3-kit.dll                                    $PWD/../data
cp    $BIN_DIR/robots-ev3-generator-base.dll                         $PWD/../data
cp    $BIN_DIR/qextserialport.dll                                    $PWD/../data
cp    $BIN_DIR/libusb-1.0.dll                                        $PWD/../data
