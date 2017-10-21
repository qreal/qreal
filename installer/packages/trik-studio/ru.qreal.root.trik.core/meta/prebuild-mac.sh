#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

mkdir -p "$BUNDLE_CONTENTS/Lib/plugins/editors"
cp     $BIN_DIR/plugins/editors/libtrikMetamodel.dylib                                "$BUNDLE_CONTENTS/Lib/plugins/editors/"
cp -pR $BIN_DIR/librobots-trik-qts-generator-library*.dylib                           "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/librobots-trik-generator-base*.dylib                                  "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/librobots-trik-kit*.dylib                                             "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/librobots-trik-kit-interpreter-common*.dylib                          "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libtrik*-x86*.dylib                                                   "$BUNDLE_CONTENTS/Lib"

fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/plugins/editors/libtrikMetamodel.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-trik-qts-generator-library.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-trik-generator-base.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-trik-kit.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-trik-kit-interpreter-common.1.0.0.dylib"

mkdir -p "$BUNDLE_CONTENTS/MacOS"
rsync -a  $BIN_DIR/system.js                                                 "$BUNDLE_CONTENTS/MacOS/"


cd "$(dirname "$0")"/../data
rm -rf winscp
mkdir -p "$BUNDLE_CONTENTS/Resources/trikRuntime"
cd "$BUNDLE_CONTENTS/Resources/trikRuntime"
tar xfvz ../../../../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../../../../trik-runtime-builds
