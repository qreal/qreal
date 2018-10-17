#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $QT_DIR/bin/Qt5MultimediaWidgets.dll  $PWD/../data
cp    $QT_DIR/bin/Qt5OpenGL.dll             $PWD/../data

cd $PWD/../data/gamepad-build
$QT_DIR/bin/qmake -nocache CONFIG+=release
mingw32-make -j4

cd ..
mv gamepad-build/release/gamepad.exe .
rm -rf gamepad-build
