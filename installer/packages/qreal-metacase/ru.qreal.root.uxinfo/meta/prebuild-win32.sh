#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/uxInfo.dll                        $PWD/../data/plugins/tools/
