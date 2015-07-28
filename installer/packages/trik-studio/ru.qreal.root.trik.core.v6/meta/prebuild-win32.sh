#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-v6-interpreter.dll            $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-v6-qts-generator.dll          $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/robots-trik-v6-runtime-uploader-plugin.dll           $PWD/../data/plugins/tools/
