#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


fix_dependencies $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-interpreter.dylib         $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-osek-c-generator.dylib    $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-russian-c-generator.dylib $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-nxt-generator-base.1.0.0.dylib                         $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/librobots-nxt-kit.1.0.0.dylib                                    $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqextserialport.1.0.0.dylib                                    $QT_DIR/lib @executable_path/../../..

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-interpreter.dylib                   $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-osek-c-generator.dylib              $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-russian-c-generator.dylib           $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-nxt-generator-base*.dylib                                        $PWD/../data
cp -pr $BIN_DIR/librobots-nxt-kit*.dylib                                                   $PWD/../data
cp -pr $BIN_DIR/libqextserialport*.dylib                                                   $PWD/../data
