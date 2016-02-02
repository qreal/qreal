#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/interpreted/generationRulesTool.dll                        $PWD/../data/plugins/interpreted/
