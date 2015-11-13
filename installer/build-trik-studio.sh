#!/bin/bash
# Usage: build-trik-studio.sh <path-to-qt> <path-to-qt-installer-framework> [tag]. See build-installer.sh for more info.

set -o nounset
set -o errexit

cd "$(dirname "$0")"

export QREAL_BUILD_TAG=${3:-}

[ $OSTYPE == "linux-gnu" ] && EXCLUDE="-e ru.qreal.root.nxt.tools,ru.qreal.root.associations" || EXCLUDE=
grep -q "darwin" <<< $OSTYPE && EXCLUDE="-e ru.qreal.root.nxt.tools,ru.qreal.root.associations" || :
grep -q "darwin" <<< $OSTYPE && export PRODUCT_DISPLAYED_NAME="TRIK Studio" || :
chmod +x $PWD/build-installer.sh && $PWD/build-installer.sh $1 $2 trik-studio $EXCLUDE
