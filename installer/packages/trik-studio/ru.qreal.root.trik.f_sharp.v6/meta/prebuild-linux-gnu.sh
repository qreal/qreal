#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v6-f-sharp-generator.so               $PWD/../data/plugins/tools/kitPlugins/
