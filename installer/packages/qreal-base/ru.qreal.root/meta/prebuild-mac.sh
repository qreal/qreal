#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp -pr $BIN_DIR/libqrkernel*.dylib                                         $PWD/../data
cp -pr $BIN_DIR/libqrrepo*.dylib                                           $PWD/../data
cp -pr $BIN_DIR/libqrutils*.dylib                                          $PWD/../data
cp -pr $BIN_DIR/libqrtext*.dylib                                           $PWD/../data
cp -pr $BIN_DIR/libqscintilla2*.dylib                                      $PWD/../data
cp -pr $BIN_DIR/libqslog*.dylib                                            $PWD/../data
cp -pr $BIN_DIR/libqrgui-brand-manager*.dylib                              $PWD/../data
cp -pr $BIN_DIR/libqrgui-controller*.dylib                                 $PWD/../data
cp -pr $BIN_DIR/libqrgui-dialogs*.dylib                                    $PWD/../data
cp -pr $BIN_DIR/libqrgui-editor*.dylib                                     $PWD/../data
cp -pr $BIN_DIR/libqrgui-hotkey-manager*.dylib                             $PWD/../data
cp -pr $BIN_DIR/libqrgui-models*.dylib                                     $PWD/../data
cp -pr $BIN_DIR/libqrgui-mouse-gestures*.dylib                             $PWD/../data
cp -pr $BIN_DIR/libqrgui-plugin-manager*.dylib                             $PWD/../data
cp -pr $BIN_DIR/libqrgui-preferences-dialog*.dylib                         $PWD/../data
cp -pr $BIN_DIR/libqrgui-text-editor*.dylib                                $PWD/../data
cp -pr $BIN_DIR/libqrgui-thirdparty*.dylib                                 $PWD/../data
cp -pr $BIN_DIR/libqrgui-tool-plugin-interface*.dylib                      $PWD/../data
cp -pr $BIN_DIR/libqrgui-facade*.dylib                                     $PWD/../data
cp -pr $BIN_DIR/qreal.app                                                  "$PWD/../data/$PRODUCT_DISPLAYED_NAME.app"
cp     $BIN_DIR/plugins/tools/libupdatesChecker.dylib                      $PWD/../data/plugins/tools


BUNDLE_CONTENTS=$PWD/../data/$PRODUCT_DISPLAYED_NAME.app/Contents
mv "$BUNDLE_CONTENTS/MacOS/qreal" "$BUNDLE_CONTENTS/MacOS/$PRODUCT"
sed -i.bak s/qreal/trik-studio/g "$BUNDLE_CONTENTS/Info.plist"
sed -i.bak s/yourcompany/cybertech/g "$BUNDLE_CONTENTS/Info.plist"
rm -f "$BUNDLE_CONTENTS/Info.plist.bak"

function copyQtLib {
	PATH_TO_QT_LIB=$QT_DIR/lib/$1.framework/Versions/5/$1
	OLD_PATH=`pwd`
	cd `dirname $PATH_TO_QT_LIB`
	NORMALIZED_PATH_TO_QT_LIB=`pwd`/$1
	cd $OLD_PATH
	cp -pr $PATH_TO_QT_LIB $PWD/../data/$1.dylib
	install_name_tool -change $NORMALIZED_PATH_TO_QT_LIB $PWD/../data/$1.dylib "$BUNDLE_CONTENTS/MacOS/$PRODUCT"
}

copyQtLib QtCore
copyQtLib QtDBus
copyQtLib QtGui
copyQtLib QtPrintSupport
copyQtLib QtSvg
copyQtLib QtWidgets
copyQtLib QtXml
copyQtLib QtScript
copyQtLib QtTest

cp     $QT_DIR/plugins/imageformats/libqsvg.dylib                          $PWD/../data/imageformats
cp     $QT_DIR/plugins/iconengines/libqsvgicon.dylib                       $PWD/../data/iconengines
