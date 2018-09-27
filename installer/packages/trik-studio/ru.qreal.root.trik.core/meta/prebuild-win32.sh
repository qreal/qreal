#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/trikMetamodel.dll                                  $PWD/../data/plugins/editors/
cp    $BIN_DIR/robots-trik-qts-generator-library.dll                              $PWD/../data/
cp    $BIN_DIR/robots-trik-generator-base.dll                                     $PWD/../data/
cp    $BIN_DIR/robots-trik-kit.dll                                                $PWD/../data/
cp    $BIN_DIR/robots-trik-kit-interpreter-common.dll                             $PWD/../data/

cp    $BIN_DIR/qslog.dll                                                          $PWD/../data/
cp    $BIN_DIR/PythonQt-*.dll                                                     $PWD/../data/
cp    $BIN_DIR/trikControl.dll                                                    $PWD/../data/
cp    $BIN_DIR/trikScriptRunner.dll                                               $PWD/../data/
cp    $BIN_DIR/trikHal.dll                                                        $PWD/../data/
cp    $BIN_DIR/trikKernel.dll                                                     $PWD/../data/
cp    $BIN_DIR/trikNetwork.dll                                                    $PWD/../data/
cp    $BIN_DIR/python27.dll                                                       $PWD/../data/


cp    $BIN_DIR/system.js                                                          $PWD/../data/bin/
cp    $BIN_DIR/system.py                                                          $PWD/../data/bin/

cd "$(dirname "$0")"/../data
rm -rf trik-runtime-builds
git clone  --depth 1 https://github.com/qreal/trik-runtime-builds
mv trik-runtime-builds/winscp $PWD
rm -rf trik-runtime-builds
