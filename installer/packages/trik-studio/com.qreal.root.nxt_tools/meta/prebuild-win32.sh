#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/

git config --global core.autocrlf input
cd $PWD/../data
rm -rf nxt-tools
git clone --depth=0 https://github.com/qreal/nxt-tools.git
