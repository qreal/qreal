#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


mkdir -p $PWD/../data/lib/plugins/tools/kitPlugins
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-ev3-interpreter.so   $PWD/../data/lib/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-ev3-rbf-generator.so $PWD/../data/lib/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-ev3-kit.so*                                   $PWD/../data/lib
cp -pr $BIN_DIR/librobots-ev3-generator-base.so*                        $PWD/../data/lib
cp -pr $BIN_DIR/libqextserialport.so*                                   $PWD/../data/lib

cp -L  `dpkg -L libusb-1.0-0-dev | grep .so`                            $PWD/../data/lib/libusb-1.0.so

# Cleaning up prebuild-common.sh results...
rm -rf $PWD/../data/plugins/
mv $PWD/../data/examples $PWD/../data/resources

cd $PWD/../data/lib/
ln -s libusb-1.0.so libusb-1.0.so.0
