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

cp    $BIN_DIR/qslog-x86.dll                                                      $PWD/../data/
cp    $BIN_DIR/trikControl-x86.dll                                                $PWD/../data/
cp    $BIN_DIR/trikScriptRunner-x86.dll                                           $PWD/../data/
cp    $BIN_DIR/trikHal-x86.dll                                                    $PWD/../data/
cp    $BIN_DIR/trikKernel-x86.dll                                                 $PWD/../data/
cp    $BIN_DIR/trikNetwork-x86.dll                                                $PWD/../data/

cp    $BIN_DIR/system.js                                                          $PWD/../data/bin/

cd "$(dirname "$0")"/../data
rm -rf winscp
mv trik-runtime-builds/winscp $PWD
mkdir -p trikRuntime
cd trikRuntime
tar xfvz ../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../trik-runtime-builds
