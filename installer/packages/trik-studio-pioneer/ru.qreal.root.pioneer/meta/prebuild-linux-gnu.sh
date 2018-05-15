#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/plugins/editors/
mkdir -p $PWD/../data/lib/plugins/tools/kitPlugins/

cp $BIN_DIR/librobots-pioneer-kit.so*					$PWD/../data/lib/
cp $BIN_DIR/plugins/editors/libpioneerMetamodel.so			$PWD/../data/lib/plugins/editors/
cp $BIN_DIR/plugins/tools/kitPlugins/librobots-pioneer-interpreter.so	$PWD/../data/lib/plugins/tools/kitPlugins/
cp $BIN_DIR/plugins/tools/kitPlugins/librobots-pioneer-lua-generator.so	$PWD/../data/lib/plugins/tools/kitPlugins/
