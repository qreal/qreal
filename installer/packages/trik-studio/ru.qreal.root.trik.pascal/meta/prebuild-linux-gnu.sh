#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/

cp    $BIN_DIR/librobots-trik-pascal-abc-generator-library.so                              $PWD/../data/lib/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-v62-pascal-abc-generator.so            $PWD/../data/lib/plugins/tools/kitPlugins/

# Cleaning up prebuild-common.sh results...
mkdir -p $PWD/../data/resources/
mv $PWD/../data/Trik.Core.dll                   $PWD/../data/resources/
mv $PWD/../data/trikSharp/FSharp.Core.dll       $PWD/../data/resources/
mv $PWD/../data/trikSharp/libconWrap.so.1.0.0   $PWD/../data/resources/
