#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/

cp -pr $QT_DIR/lib/libQt5MultimediaWidgets.so*		$PWD/../data/lib/
cp -pr $QT_DIR/lib/libQt5OpenGL.so*					$PWD/../data/lib/

cd gamepad-build
$QT_DIR/bin/qmake -nocache CONFIG+=release
make -j4

cd ..
rm -rf ../data/gamepad
mv gamepad-build/gamepad ../data/
rm -rf gamepad-build
