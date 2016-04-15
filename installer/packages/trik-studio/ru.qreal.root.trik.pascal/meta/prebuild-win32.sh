#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/robots-trik-pascal-abc-generator-library.dll                              $PWD/../data/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-v62-pascal-abc-generator.dll         $PWD/../data/plugins/tools/kitPlugins/
