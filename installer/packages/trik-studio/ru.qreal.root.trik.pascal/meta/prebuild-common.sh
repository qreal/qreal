#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/tools/kitPlugins/
mkdir -p $PWD/../data/trikSharp/

cp     $BIN_DIR/trikSharp/Trik.Core.dll                       $PWD/../data/trikSharp/
cp     $BIN_DIR/trikSharp/FSharp.Core.dll                     $PWD/../data/trikSharp/
cp     $BIN_DIR/trikSharp/libconWrap.so.1.0.0                 $PWD/../data/trikSharp/
