#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/plugins/tools
mkdir -p $PWD/../data/lib/plugins/editors


cp -r  $BIN_DIR/plugins/*                $PWD/../data/lib/plugins
