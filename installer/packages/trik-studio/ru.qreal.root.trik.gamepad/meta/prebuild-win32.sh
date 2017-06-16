#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $QT_DIR/bin/Qt5MultimediaWidgets.dll	$PWD/../data
cp    $QT_DIR/bin/Qt5OpenGL.dll				$PWD/../data

git clone --depth 1 https://github.com/trikset/trik-desktop-gamepad.git gamepad-build

cd gamepad-build
$QT_DIR/bin/qmake CONFIG+=release
mingw32-make -j4

cd ..
rm -rf ../data/gamepad.exe
mv gamepad-build/release/gamepad.exe ../data/
rm -rf gamepad-build
