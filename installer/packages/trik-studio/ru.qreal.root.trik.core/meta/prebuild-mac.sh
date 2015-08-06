#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


fix_dependencies $BIN_DIR/librobots-trik-qts-generator-library.1.0.0.dylib            $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-trik-generator-base.1.0.0.dylib                   $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-trik-kit.1.0.0.dylib                              $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-trik-kit-interpreter-common.1.0.0.dylib           $QT_DIR/lib @executable_path/../../..

cp -pr $BIN_DIR/librobots-trik-qts-generator-library*.dylib                           $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-generator-base*.dylib                                  $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit*.dylib                                             $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit-interpreter-common*.dylib                          $PWD/../data/

copy_qt_lib QtNetwork
