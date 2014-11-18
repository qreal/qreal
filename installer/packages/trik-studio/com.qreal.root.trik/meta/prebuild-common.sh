#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $INSTALLER_ROOT/licenses/WinScp-license.txt                        $PWD


mkdir -p $PWD/../data/plugins/tools/kitPlugins
mkdir -p $PWD/../data/examples
cp -r  $BIN_DIR/examples/trik                                             $PWD/../data/examples
cd $PWD/../data/
rm -rf trik-runtime-builds
git clone --depth=0 https://github.com/qreal/trik-runtime-builds
rm -rf winscp
mv trik-runtime-builds/winscp $PWD
mkdir -p trikRuntime
cd trikRuntime
tar xfvzh ../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../trik-runtime-builds
