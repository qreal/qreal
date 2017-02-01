#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

mkdir -p "$BUNDLE_CONTENTS/Lib/"

cp    $BIN_DIR/librobots-trik-pascal-abc-generator-library.dylib                          "$BUNDLE_CONTENTS/Lib/"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-trik-pascal-abc-generator-library.dylib"

mkdir -p "$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins"
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-pascal-abc-generator.dylib    "$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins/"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins/librobots-trik-v62-pascal-abc-generator.dylib"

# Cleaning up prebuild-common.sh results...
mkdir -p $PWD/../data/resources/
mv $PWD/../data/Trik.Core.dll                   $PWD/../data/resources/
mv $PWD/../data/trikSharp/FSharp.Core.dll       $PWD/../data/resources/
mv $PWD/../data/trikSharp/libconWrap.so.1.0.0   $PWD/../data/resources/
