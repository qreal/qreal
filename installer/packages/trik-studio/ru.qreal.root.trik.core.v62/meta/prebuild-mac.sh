#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


fix_dependencies $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-interpreter.dylib     $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-qts-generator.dylib   $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/plugins/tools/librobots-trik-v62-runtime-uploader-plugin.dylib    $QT_DIR/lib @executable_path/../../..

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-interpreter.dylib               $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-qts-generator.dylib             $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/librobots-trik-v62-runtime-uploader-plugin.dylib              $PWD/../data/plugins/tools/
