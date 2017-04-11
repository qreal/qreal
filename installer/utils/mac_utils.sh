#!/bin/bash
set -o nounset
set -o errexit

export BUNDLE_CONTENTS=$PWD/../data/$PRODUCT_DISPLAYED_NAME.app/Contents/
export LIB_PATH=@executable_path/../Lib

function fix_dependencies {
	:
}

function fix_qreal_dependencies {
	:
}

function copy_qt_lib {
	cp -R "$QT_DIR/lib/$1.framework/Versions/5/$1" "$BUNDLE_CONTENTS/Lib/$1"
	fix_dependencies "$BUNDLE_CONTENTS/Lib/$1" "$QT_DIR/lib" "$LIB_PATH"
}
