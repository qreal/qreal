#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp -pr $BIN_DIR/libqrkernel.so*                                         $PWD/../data
cp -pr $BIN_DIR/libqrrepo.so*                                           $PWD/../data
cp -pr $BIN_DIR/libqrutils.so*                                          $PWD/../data
cp -pr $BIN_DIR/libqrtext.so*                                           $PWD/../data
cp -pr $BIN_DIR/libqscintilla2.so*                                      $PWD/../data
cp -pr $BIN_DIR/libqslog.so*                                            $PWD/../data
cp -pr $BIN_DIR/libqrgui-brand-manager.so*                              $PWD/../data
cp -pr $BIN_DIR/libqrgui-controller.so*                                 $PWD/../data
cp -pr $BIN_DIR/libqrgui-dialogs.so*                                    $PWD/../data
cp -pr $BIN_DIR/libqrgui-editor.so*                                     $PWD/../data
cp -pr $BIN_DIR/libqrgui-hotkey-manager.so*                             $PWD/../data
cp -pr $BIN_DIR/libqrgui-models.so*                                     $PWD/../data
cp -pr $BIN_DIR/libqrgui-mouse-gestures.so*                             $PWD/../data
cp -pr $BIN_DIR/libqrgui-plugin-manager.so*                             $PWD/../data
cp -pr $BIN_DIR/libqrgui-preferences-dialog.so*                         $PWD/../data
cp -pr $BIN_DIR/libqrgui-text-editor.so*                                $PWD/../data
cp -pr $BIN_DIR/libqrgui-thirdparty.so*                                 $PWD/../data
cp -pr $BIN_DIR/libqrgui-tool-plugin-interface.so*                      $PWD/../data
cp     $BIN_DIR/qrgui                                                   $PWD/../data/$PRODUCT


cp -pr $QT_DIR/lib/libQt5Core.so*                                       $PWD/../data
cp -pr $QT_DIR/lib/libQt5DBus.so*                                       $PWD/../data
cp -pr $QT_DIR/lib/libQt5Gui.so*                                        $PWD/../data
cp -pr $QT_DIR/lib/libQt5PrintSupport.so*                               $PWD/../data
cp -pr $QT_DIR/lib/libQt5Svg.so*                                        $PWD/../data
cp -pr $QT_DIR/lib/libQt5Widgets.so*                                    $PWD/../data
cp -pr $QT_DIR/lib/libQt5X11Extras.so*                                  $PWD/../data
cp -pr $QT_DIR/lib/libQt5Xml.so*                                        $PWD/../data
cp -pr $QT_DIR/lib/libicudata.so*                                       $PWD/../data
cp -pr $QT_DIR/lib/libicui18n.so*                                       $PWD/../data
cp -pr $QT_DIR/lib/libicuuc.so*                                         $PWD/../data

cp     $QT_DIR/plugins/platforms/libqxcb.so                             $PWD/../data/platforms
cp     $QT_DIR/plugins/imageformats/libqsvg.so                          $PWD/../data/imageformats
cp     $QT_DIR/plugins/iconengines/libqsvgicon.so                       $PWD/../data/iconengines

cp -pr /usr/lib/libstdc++.so*                                           $PWD/../data
