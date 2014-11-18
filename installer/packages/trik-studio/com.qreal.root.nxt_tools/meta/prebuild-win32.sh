#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/

git config --global core.autocrlf input
cd $PWD/../data
git clone https://github.com/qreal/nxt-tools.git
