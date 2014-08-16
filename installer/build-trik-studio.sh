#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


# Usage: build-trik-studio.sh <path-to-qt>. See build-installer.sh for more info.

[ $OSTYPE == "linux-gnu" ] && EXCLUDE="-e com.qreal.root.robots.nxt_tools" || EXCLUDE=
chmod +x $PWD/build-installer.sh && $PWD/build-installer.sh $@ trik-studio $EXCLUDE
