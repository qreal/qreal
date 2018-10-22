#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

mkdir -p "$BUNDLE_CONTENTS/Lib"

#cp -pr $QT_DIR/lib/libQt5MultimediaWidgets.so*		$PWD/../data/lib/
#cp -pr $QT_DIR/lib/libQt5OpenGL.so*			$PWD/../data/lib/


cd gamepad-build
$QT_DIR/bin/qmake CONFIG+=release
make -j4

cd ..
mkdir -p "$PWD/../data/"
rsync -av gamepad-build/gamepad.app/ "$PWD/../data/gamepad.app/"
copy_qt_lib QtMultimediaWidgets
ln -vsf "../TRIK Studio.app/Contents/Lib" "$PWD/../data/gamepad.app/Contents/Lib"
#rm -rf gamepad-build
