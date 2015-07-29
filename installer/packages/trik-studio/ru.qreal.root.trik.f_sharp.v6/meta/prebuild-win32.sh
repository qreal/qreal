#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-v6-f-sharp-generator.dll             $PWD/../data/plugins/tools/kitPlugins/
