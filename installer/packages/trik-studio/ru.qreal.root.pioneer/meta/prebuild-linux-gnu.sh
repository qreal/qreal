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

cp $BIN_DIR/pioneerStart.sh	$PWD/../data/
cp $BIN_DIR/pioneerUpload.sh	$PWD/../data/
cp $BIN_DIR/pioneerCompile.sh	$PWD/../data/
cp $BIN_DIR/controller		$PWD/../data/

mkdir -p $PWD/../data/luac/real

cp $BIN_DIR/luac/real/luac5.3 $PWD/../data/luac/real/
chmod +x $PWD/../data/luac/real/
