#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-ev3-interpreter.so   $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugnis/tools/kitPlugins/libev3-rbf-generator.so        $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-ev3-kit.so*                                   $PWD/../data
cp -pr $BIN_DIR/librobots-ev3-generator-base.so*                        $PWD/../data
cp -pr $BIN_DIR/libqextserialport.so*                                   $PWD/../data

cp -pr /usr/bin/libusb*                                                 $PWD/../data
