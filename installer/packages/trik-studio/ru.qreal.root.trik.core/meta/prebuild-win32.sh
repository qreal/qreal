#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/robots-trik-qts-generator-library.dll                              $PWD/../data/
cp    $BIN_DIR/robots-trik-generator-base.dll                                     $PWD/../data/
cp    $BIN_DIR/robots-trik-kit.dll                                                $PWD/../data/
cp    $BIN_DIR/robots-trik-kit-interpreter-common.dll                             $PWD/../data/

cp    $QT_DIR/bin/Qt5Network.dll                                                  $PWD/../data/

cd "$(dirname "$0")"/../data
rm -rf winscp
mv trik-runtime-builds/winscp $PWD
mkdir -p trikRuntime
cd trikRuntime
tar xfvzh ../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../trik-runtime-builds
