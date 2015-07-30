#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp -pr $BIN_DIR/librobots-trik-qts-generator-library*.dylib                           $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-generator-base*.dylib                                  $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit*.dylib                                             $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit-interpreter-common*.dylib                          $PWD/../data/

BUNDLE_CONTENTS=$PWD/../../../qreal-base/ru.qreal.root/data/$PRODUCT_DISPLAYED_NAME.app/Contents
function copyQtLib {
	PATH_TO_QT_LIB=$QT_DIR/lib/$1.framework/Versions/5/$1
	OLD_PATH=`pwd`
	cd `dirname $PATH_TO_QT_LIB`
	NORMALIZED_PATH_TO_QT_LIB=`pwd`/$1
	cd $OLD_PATH
	cp -pr $PATH_TO_QT_LIB $PWD/../data/$1.dylib
	install_name_tool -change $NORMALIZED_PATH_TO_QT_LIB $PWD/../data/$1.dylib "$BUNDLE_CONTENTS/MacOS/$PRODUCT"
}

copyQtLib QtNetwork
