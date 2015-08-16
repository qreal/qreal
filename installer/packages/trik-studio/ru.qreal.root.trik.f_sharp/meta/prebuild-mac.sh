#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

fix_dependencies $BIN_DIR/librobots-trik-f-sharp-generator-library.dylib                  $QT_DIR/lib @executable_path/../../..
cp     $BIN_DIR/librobots-trik-f-sharp-generator-library.dylib                            $PWD/../data/
