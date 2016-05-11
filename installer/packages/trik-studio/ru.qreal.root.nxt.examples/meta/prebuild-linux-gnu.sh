#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/resources
mv $PWD/../data/examples $PWD/../data/resources/
