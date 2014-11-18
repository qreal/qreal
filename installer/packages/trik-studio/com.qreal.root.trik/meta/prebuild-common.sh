#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


mkdir -p $PWD/../data/plugins/tools/kitPlugins
mkdir -p $PWD/../data/examples
cp -r  $BIN_DIR/examples/trik                                             $PWD/../data/examples
cd $PWD/../data/
git clone https://github.com/qreal/trik-runtime-builds
mkdir trikRuntime
cd trikRuntime
tar xJfv ../trikRuntime.tar.xz