#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/resources/examples
rsync -av --remove-source-files $PWD/../data/examples/ $PWD/../data/resources/examples/
