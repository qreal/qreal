#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/common_utils.sh


mkdir -p $PWD/../data/

git config --global core.autocrlf input
cd $PWD/../data
rm -rf nxt-tools
git clone --depth 1 https://github.com/qreal/nxt-tools.git
cd nxt-tools
[ -z "${QREAL_BUILD_TAG:-}" ] && : || { checkout_tag_or_warn $QREAL_BUILD_TAG "NXT Tools"; }
rm -rf .git
git config --global core.autocrlf true
