#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp -pr $BIN_DIR/libqrkernel.so*                                         $PWD/../data
cp -pr $BIN_DIR/libqrrepo.so*                                           $PWD/../data
cp -pr $BIN_DIR/libqrutils.so*                                          $PWD/../data
cp -pr $BIN_DIR/libqscintilla2.so*                                      $PWD/../data
cp -pr $BIN_DIR/libqslog.so*                                            $PWD/../data
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

cp -r  $QT_DIR/plugins/iconengines                                      $PWD/../data
cp -r  $QT_DIR/plugins/imageformats                                     $PWD/../data #TODO: not all needed?
cp -r  $QT_DIR/plugins/platforms                                        $PWD/../data #TODO: not all needed?

cp -pr /usr/lib/libstdc++.so*                                           $PWD/../data
