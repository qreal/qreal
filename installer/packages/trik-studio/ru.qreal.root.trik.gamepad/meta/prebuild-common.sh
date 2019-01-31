#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

rm -rf $PWD/../data/
mkdir -p $PWD/../data/

git clone --depth 1 https://github.com/trikset/trik-desktop-gamepad.git $PWD/../data/gamepad-build
cd $PWD/../data/gamepad-build
git submodule update --init
cd $PWD/../../meta
