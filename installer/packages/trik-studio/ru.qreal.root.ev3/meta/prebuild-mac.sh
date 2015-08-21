#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


fix_dependencies $BIN_DIR/plugins/tools/kitPlugins/librobots-ev3-interpreter.dylib $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-ev3-kit.1.0.0.dylib                            $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqextserialport.1.0.0.dylib                            $QT_DIR/lib @executable_path/../../..

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-ev3-interpreter.dylib   $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-ev3-kit*.dylib                                   $PWD/../data
cp -pr $BIN_DIR/libqextserialport*.dylib                                   $PWD/../data