#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

mkdir -p "$BUNDLE_CONTENTS/Lib/"
cp     $BIN_DIR/librobots-trik-f-sharp-generator-library.dylib                            "$BUNDLE_CONTENTS/Lib/"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-trik-f-sharp-generator-library.dylib"

# Cleaning up prebuild-common.sh results...
mkdir -p $PWD/../data/resources/
mv $PWD/../data/Trik.Core.dll $PWD/../data/resources/
