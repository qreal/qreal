#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


fix_dependencies $BIN_DIR/plugins/editors/librobotsMetamodel.dylib         $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/plugins/tools/librobots-plugin.dylib             $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-interpreter-core.1.0.0.dylib           $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-2d-model.1.0.0.dylib                   $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-generator-base.1.0.0.dylib             $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-kit-base.1.0.0.dylib                   $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-utils.1.0.0.dylib                      $QT_DIR/lib @executable_path/../../..

cp     $BIN_DIR/plugins/editors/librobotsMetamodel.dylib                   $PWD/../data/plugins/editors/
cp     $BIN_DIR/plugins/tools/librobots-plugin.dylib                       $PWD/../data/plugins/tools/
cp -pr $BIN_DIR/librobots-interpreter-core*.dylib                          $PWD/../data
cp -pr $BIN_DIR/librobots-2d-model*.dylib                                  $PWD/../data
cp -pr $BIN_DIR/librobots-generator-base*.dylib                            $PWD/../data
cp -pr $BIN_DIR/librobots-kit-base*.dylib                                  $PWD/../data
cp -pr $BIN_DIR/librobots-utils*.dylib                                     $PWD/../data
