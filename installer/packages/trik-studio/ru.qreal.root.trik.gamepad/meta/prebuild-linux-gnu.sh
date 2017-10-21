#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/

cp -pr $QT_DIR/lib/libQt5MultimediaWidgets.so*		$PWD/../data/lib/
cp -pr $QT_DIR/lib/libQt5OpenGL.so*			$PWD/../data/lib/

git clone --depth 1 https://github.com/trikset/trik-desktop-gamepad.git gamepad-build

cd gamepad-build
$QT_DIR/bin/qmake CONFIG+=release
make -j4

cd ..
rm -rf ../data/gamepad
mv gamepad-build/gamepad ../data/
rm -rf gamepad-build
