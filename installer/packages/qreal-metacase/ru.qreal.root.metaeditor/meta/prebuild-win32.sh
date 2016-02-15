#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/editors/metaEditor.dll                        $PWD/../data/plugins/editors/
