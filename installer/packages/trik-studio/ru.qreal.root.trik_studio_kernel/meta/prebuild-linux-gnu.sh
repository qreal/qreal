#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/plugins/editors/
mkdir -p $PWD/../data/lib/plugins/tools/
mkdir -p $PWD/../data/bin/
mkdir -p $PWD/../data/resources/

cp -pr $QT_DIR/lib/libQt5Multimedia.so*		$PWD/../data/lib/
cp -pr $QT_DIR/lib/libQt5Network.so*		$PWD/../data/lib/

cp     $BIN_DIR/plugins/editors/librobotsMetamodel.so                   $PWD/../data/lib/plugins/editors/
cp -pr $BIN_DIR/librobots-interpreter-core.so*                          $PWD/../data/lib/
cp     $BIN_DIR/plugins/tools/librobots-plugin.so                       $PWD/../data/lib/plugins/tools/
cp -pr $BIN_DIR/librobots-2d-model.so*                                  $PWD/../data/lib/
cp -pr $BIN_DIR/Box2D.so*                                               $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-generator-base.so*                            $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-kit-base.so*                                  $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-utils.so*                                     $PWD/../data/lib/
cp -pr $BIN_DIR/libqextserialport.so*                                   $PWD/../data/lib/

# Cleaning up prebuild-common.sh results...
rm -rf $PWD/../data/plugins/
rsync -a --remove-source-files $PWD/../data/help/ $PWD/../data/resources/help/
rsync -a --remove-source-files $PWD/../data/images/ $PWD/../data/resources/images/
mv $PWD/../data/splashscreen.png $PWD/../data/bin/
