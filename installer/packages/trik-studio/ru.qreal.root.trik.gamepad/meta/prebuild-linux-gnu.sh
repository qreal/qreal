#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

git clone --depth 1 https://github.com/trikset/trik-desktop-gamepad.git gamepad-build

cd gamepad-build
qmake CONFIG+=release
make -j4

cd ..
rm -rf ../data/gamepad
mv gamepad-build/gamepad ../data/
rm -rf gamepad-build
