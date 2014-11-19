#!/bin/bash
# Usage: build-trik-studio.sh <path-to-qt> <path-to-qt-installer-framework>. See build-installer.sh for more info.

set -o nounset
set -o errexit

cd "$(dirname "$0")"

[ $OSTYPE == "linux-gnu" ] && EXCLUDE="-e com.qreal.root.nxt_tools,com.qreal.root.associations" || EXCLUDE=
chmod +x $PWD/build-installer.sh && $PWD/build-installer.sh $@ trik-studio $EXCLUDE
