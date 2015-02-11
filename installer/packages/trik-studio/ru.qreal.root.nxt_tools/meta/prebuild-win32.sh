#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


function checkout_tag_or_warn { { PATTERN=$'\n'$1$'\n'; echo "$'\n'$(git tag)$'\n'" | grep -E $PATTERN; } && git checkout $1 || echo "Warning: NXT Tools doesn't have tag "$1; }

mkdir -p $PWD/../data/

git config --global core.autocrlf input
cd $PWD/../data
rm -rf nxt-tools
git clone --depth 1 https://github.com/qreal/nxt-tools.git
[ -z "${QREAL_BUILD_TAG:-}" ] && : || { cd nxt-tools; checkout_tag_or_warn $QREAL_BUILD_TAG; }
rm -rf nxt-tools/.git
git config --global core.autocrlf true
