#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/qrkernel.dll                                         $PWD/../data
cp    $BIN_DIR/qrrepo.dll                                           $PWD/../data
cp    $BIN_DIR/qrutils.dll                                          $PWD/../data
cp    $BIN_DIR/qscintilla2.dll                                      $PWD/../data
cp    $BIN_DIR/qslog.dll                                            $PWD/../data
cp    $BIN_DIR/qrgui.exe                                            $PWD/../data/$PRODUCT.exe


cp    $QT_DIR/lib/Qt5Core.dll                                       $PWD/../data
cp    $QT_DIR/lib/Qt5Gui.dll                                        $PWD/../data
cp    $QT_DIR/lib/Qt5PrintSupport.dll                               $PWD/../data
cp    $QT_DIR/lib/Qt5Svg.dll                                        $PWD/../data
cp    $QT_DIR/lib/Qt5Widgets.dll                                    $PWD/../data
cp    $QT_DIR/lib/Qt5Xml.dll                                        $PWD/../data
cp    $QT_DIR/lib/icudata.dll                                       $PWD/../data
cp    $QT_DIR/lib/icuin51.dll                                       $PWD/../data
cp    $QT_DIR/lib/icuuc51.dll                                       $PWD/../data
cp    $QT_DIR/lib/icudt51.dll                                       $PWD/../data
cp    $QT_DIR/lib/libgcc_s_dw2-1.dll                                $PWD/../data
cp    $QT_DIR/lib/libwinpthread-1.dll                               $PWD/../data
cp    $QT_DIR/lib/libstdc++-6.dll                                   $PWD/../data

cp    $QT_DIR/plugins/platforms/qwindows.dll                        $PWD/../data/platforms
cp    $QT_DIR/plugins/imageformats/qsvg.dll                         $PWD/../data/imageformats
cp    $QT_DIR/plugins/iconengines/qsvgicon.dll                      $PWD/../data/iconengines
