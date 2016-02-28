#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/qrkernel.dll                                         $PWD/../data
cp    $BIN_DIR/qrrepo.dll                                           $PWD/../data
cp    $BIN_DIR/qrutils.dll                                          $PWD/../data
cp    $BIN_DIR/qrtext.dll                                           $PWD/../data
cp    $BIN_DIR/qscintilla2.dll                                      $PWD/../data
cp    $BIN_DIR/qslog.dll                                            $PWD/../data
cp    $BIN_DIR/qrgui-brand-manager.dll                              $PWD/../data
cp    $BIN_DIR/qrgui-controller.dll                                 $PWD/../data
cp    $BIN_DIR/qrgui-dialogs.dll                                    $PWD/../data
cp    $BIN_DIR/qrgui-editor.dll                                     $PWD/../data
cp    $BIN_DIR/qrgui-hotkey-manager.dll                             $PWD/../data
cp    $BIN_DIR/qrgui-models.dll                                     $PWD/../data
cp    $BIN_DIR/qrgui-mouse-gestures.dll                             $PWD/../data
cp    $BIN_DIR/qrgui-plugin-manager.dll                             $PWD/../data
cp    $BIN_DIR/qrgui-meta-meta-model.dll                            $PWD/../data
cp    $BIN_DIR/qrgui-preferences-dialog.dll                         $PWD/../data
cp    $BIN_DIR/qrgui-text-editor.dll                                $PWD/../data
cp    $BIN_DIR/qrgui-thirdparty.dll                                 $PWD/../data
cp    $BIN_DIR/qrgui-tool-plugin-interface.dll                      $PWD/../data
cp    $BIN_DIR/qrgui-facade.dll                                     $PWD/../data
cp    $BIN_DIR/qreal.exe                                            $PWD/../data/$PRODUCT.exe
cp    $BIN_DIR/plugins/tools/updatesChecker.dll                     $PWD/../data/plugins/tools/


cp    $QT_DIR/bin/Qt5Core.dll                                       $PWD/../data
cp    $QT_DIR/bin/Qt5Gui.dll                                        $PWD/../data
cp    $QT_DIR/bin/Qt5PrintSupport.dll                               $PWD/../data
cp    $QT_DIR/bin/Qt5Svg.dll                                        $PWD/../data
cp    $QT_DIR/bin/Qt5Widgets.dll                                    $PWD/../data
cp    $QT_DIR/bin/Qt5Xml.dll                                        $PWD/../data
cp    $QT_DIR/bin/Qt5Script.dll                                     $PWD/../data
cp    $QT_DIR/bin/Qt5Test.dll                                       $PWD/../data

cp    $QT_DIR/bin/icudt5*.dll                                       $PWD/../data   
cp    $QT_DIR/bin/icuin5*.dll                                       $PWD/../data
cp    $QT_DIR/bin/icuuc5*.dll                                       $PWD/../data

cp    $QT_DIR/bin/libgcc_s_dw2-1.dll                                $PWD/../data
cp    $QT_DIR/bin/libwinpthread-1.dll                               $PWD/../data
cp    $QT_DIR/bin/libstdc++-6.dll                                   $PWD/../data

cp    $QT_DIR/plugins/platforms/qwindows.dll                        $PWD/../data/platforms
cp    $QT_DIR/plugins/imageformats/qsvg.dll                         $PWD/../data/imageformats
cp    $QT_DIR/plugins/iconengines/qsvgicon.dll                      $PWD/../data/iconengines
