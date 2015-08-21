#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh


fix_dependencies $BIN_DIR/libqrkernel.1.0.0.dylib                     $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrrepo.1.0.0.dylib                       $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrutils.1.0.0.dylib                      $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrtext.1.0.0.dylib                       $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqscintilla2.8.0.2.dylib                  $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqslog.1.0.0.dylib                        $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-brand-manager.1.0.0.dylib          $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-controller.1.0.0.dylib             $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-dialogs.1.0.0.dylib                $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-editor.1.0.0.dylib                 $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-hotkey-manager.1.0.0.dylib         $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-mouse-gestures.1.0.0.dylib         $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-models.1.0.0.dylib                 $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-plugin-manager.1.0.0.dylib         $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-preferences-dialog.1.0.0.dylib     $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-text-editor.1.0.0.dylib            $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-thirdparty.1.0.0.dylib             $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-tool-plugin-interface.1.0.0.dylib  $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/libqrgui-facade.1.0.0.dylib                 $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/plugins/tools/libupdatesChecker.dylib       $QT_DIR/lib @executable_path/../../..
fix_dependencies $BIN_DIR/qreal.app/Contents/MacOS/qreal              $QT_DIR/lib @executable_path/../../..

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
cp     $BIN_DIR/plugins/tools/libupdatesChecker.dylib                      $PWD/../data/plugins/tools
cp -pr $BIN_DIR/qreal.app                                                 "$PWD/../data/$PRODUCT_DISPLAYED_NAME.app"


BUNDLE_CONTENTS=$PWD/../data/$PRODUCT_DISPLAYED_NAME.app/Contents
mv "$BUNDLE_CONTENTS/MacOS/qreal" "$BUNDLE_CONTENTS/MacOS/$PRODUCT"
sed -i.bak s/qreal/trik-studio/g "$BUNDLE_CONTENTS/Info.plist"
sed -i.bak s/yourcompany/cybertech/g "$BUNDLE_CONTENTS/Info.plist"
rm -f "$BUNDLE_CONTENTS/Info.plist.bak"

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

cp     $QT_DIR/plugins/platforms/libqcocoa.dylib                           "$BUNDLE_CONTENTS/MacOS/platforms"
cp     $QT_DIR/plugins/imageformats/libqsvg.dylib                          "$BUNDLE_CONTENTS/MacOS/imageformats"
cp     $QT_DIR/plugins/iconengines/libqsvgicon.dylib                       "$BUNDLE_CONTENTS/MacOS/iconengines"

fix_dependencies "$BUNDLE_CONTENTS/MacOS/platforms/libqcocoa.dylib"     $QT_DIR/lib @executable_path/../../..
fix_dependencies "$BUNDLE_CONTENTS/MacOS/imageformats/libqsvg.dylib"    $QT_DIR/lib @executable_path/../../..
fix_dependencies "$BUNDLE_CONTENTS/MacOS/iconengines/libqsvgicon.dylib" $QT_DIR/lib @executable_path/../../..
