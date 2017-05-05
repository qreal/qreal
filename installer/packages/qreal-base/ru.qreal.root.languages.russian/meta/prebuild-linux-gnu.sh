#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

# Cleaning up prebuild-common.sh results...
mkdir -p $PWD/../data/resources/translations
rsync -av --remove-source-files $PWD/../data/translations/ $PWD/../data/resources/translations/
