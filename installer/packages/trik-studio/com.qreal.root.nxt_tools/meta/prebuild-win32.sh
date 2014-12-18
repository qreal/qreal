#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/

git config --global core.autocrlf input
cd $PWD/../data
rm -rf nxt-tools
git clone https://github.com/qreal/nxt-tools.git
[ -z "${QREAL_BUILD_TAG:-}" ] && : || cd nxt-tools && git checkout $QREAL_BUILD_TAG
rm -rf nxt-tools/.git
git config --global core.autocrlf true
