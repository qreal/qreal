#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

# Cleaning up prebuild-common.sh results...
mkdir -p $PWD/../data/resources/
mv $PWD/../data/translations/ $PWD/../data/resources/
