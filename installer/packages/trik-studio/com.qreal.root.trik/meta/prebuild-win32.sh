#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-interpreter.dll               $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-qts-generator.dll             $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-runtime-uploader-plugin.dll   $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/robots-trik-generator-base.dll                                     $PWD/../data/


cp    $QT_DIR/bin/Qt5Network.dll                                                  $PWD/../data

cd $PWD/../data/
rm -rf trik-runtime-builds
git clone https://github.com/qreal/trik-runtime-builds
[ -z "${QREAL_BUILD_TAG:-}" ] && : || cd trik-runtime-builds && git checkout $QREAL_BUILD_TAG
rm -rf winscp
mv trik-runtime-builds/winscp $PWD
mkdir -p trikRuntime
cd trikRuntime
tar xfvzh ../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../trik-runtime-builds
