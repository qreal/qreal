#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp $BIN_DIR/robots-pioneer-kit.dll                                    $PWD/../data/
cp $BIN_DIR/plugins/editors/pioneerMetamodel.dll                      $PWD/../data/plugins/editors/
cp $BIN_DIR/plugins/tools/kitPlugins/robots-pioneer-interpreter.dll   $PWD/../data/plugins/tools/kitPlugins/
cp $BIN_DIR/plugins/tools/kitPlugins/robots-pioneer-lua-generator.dll $PWD/../data/plugins/tools/kitPlugins/
