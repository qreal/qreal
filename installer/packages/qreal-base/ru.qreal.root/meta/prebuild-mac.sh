#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


cp -R $BIN_DIR/qreal.app                                                  "$PWD/../data/$PRODUCT_DISPLAYED_NAME.app"
cp    $INSTALLER_ROOT/platform/$PRODUCT-mac-platform.config               "$BUNDLE_CONTENTS/MacOS/platform.config"
mkdir -p "$BUNDLE_CONTENTS/Lib/plugins/tools"

cp -pR $BIN_DIR/libqrkernel*.dylib                                        "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrrepo*.dylib                                          "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrutils*.dylib                                         "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrtext*.dylib                                          "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqscintilla2*.dylib                                     "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqslog*.dylib                                           "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-brand-manager*.dylib                             "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-controller*.dylib                                "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-dialogs*.dylib                                   "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-editor*.dylib                                    "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-hotkey-manager*.dylib                            "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-models*.dylib                                    "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-mouse-gestures*.dylib                            "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-plugin-manager*.dylib                            "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-meta-meta-model*.dylib                           "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-preferences-dialog*.dylib                        "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-text-editor*.dylib                               "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-thirdparty*.dylib                                "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-tool-plugin-interface*.dylib                     "$BUNDLE_CONTENTS/Lib"
cp -pR $BIN_DIR/libqrgui-facade*.dylib                                    "$BUNDLE_CONTENTS/Lib"
cp     $BIN_DIR/plugins/tools/libupdatesChecker.dylib                     "$BUNDLE_CONTENTS/Lib/plugins/tools"


fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrkernel.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrrepo.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrutils.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrtext.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqscintilla2.8.0.2.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqslog.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-brand-manager.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-controller.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-dialogs.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-editor.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-hotkey-manager.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-mouse-gestures.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-models.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-plugin-manager.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-preferences-dialog.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-text-editor.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-thirdparty.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-tool-plugin-interface.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/libqrgui-facade.1.0.0.dylib"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/plugins/tools/libupdatesChecker.dylib"

mv "$BUNDLE_CONTENTS/MacOS/qreal" "$BUNDLE_CONTENTS/MacOS/$PRODUCT"
sed -i.bak s/qreal/trik-studio/g "$BUNDLE_CONTENTS/Info.plist"
sed -i.bak s/yourcompany/cybertech/g "$BUNDLE_CONTENTS/Info.plist"
rm -f "$BUNDLE_CONTENTS/Info.plist.bak"
fix_qreal_dependencies "$BUNDLE_CONTENTS/MacOS/$PRODUCT"

copy_qt_lib QtCore
copy_qt_lib QtDBus
copy_qt_lib QtGui
copy_qt_lib QtPrintSupport
copy_qt_lib QtSvg
copy_qt_lib QtWidgets
copy_qt_lib QtXml
copy_qt_lib QtScript
copy_qt_lib QtTest

mkdir "$BUNDLE_CONTENTS/MacOS/platforms"
mkdir "$BUNDLE_CONTENTS/MacOS/imageformats"
mkdir "$BUNDLE_CONTENTS/MacOS/iconengines"

cp     $QT_DIR/plugins/platforms/libqcocoa.dylib                          "$BUNDLE_CONTENTS/MacOS/platforms"
cp     $QT_DIR/plugins/imageformats/libqsvg.dylib                         "$BUNDLE_CONTENTS/MacOS/imageformats"
cp     $QT_DIR/plugins/iconengines/libqsvgicon.dylib                      "$BUNDLE_CONTENTS/MacOS/iconengines"

fix_qreal_dependencies "$BUNDLE_CONTENTS/MacOS/platforms/libqcocoa.dylib"       $QT_DIR/lib $LIB_PATH
fix_qreal_dependencies "$BUNDLE_CONTENTS/MacOS/imageformats/libqsvg.dylib"      $QT_DIR/lib $LIB_PATH
fix_qreal_dependencies "$BUNDLE_CONTENTS/MacOS/iconengines/libqsvgicon.dylib"   $QT_DIR/lib $LIB_PATH

# Cleaning up prebuild-common.sh results…
rm -rf $PWD/../data/platforms/
rm -rf $PWD/../data/imageformats/
rm -rf $PWD/../data/iconengines/
rm -rf $PWD/../data/plugins/
mv $PWD/../data/acknowledgements.txt "$BUNDLE_CONTENTS/Resources"
mv $PWD/../data/changelog.txt "$BUNDLE_CONTENTS/Resources"
