#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/metaEditorSupport.dll                        $PWD/../data/plugins/tools/
cp    $BIN_DIR/qrmc.dll                                                   $PWD/../data/
