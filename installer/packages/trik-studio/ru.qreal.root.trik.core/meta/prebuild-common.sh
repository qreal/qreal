#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/
source $INSTALLER_ROOT/utils/common_utils.sh

cp     $INSTALLER_ROOT/licenses/WinScp-license.txt                        $PWD

cd $PWD/../data/
rm -rf trik-runtime-builds
git clone  --depth 1 https://github.com/qreal/trik-runtime-builds

[ -z "${QREAL_BUILD_TAG:-}" ] && : || { cd trik-runtime-builds; checkout_tag_or_warn $QREAL_BUILD_TAG "TRIK Runtime"; }
# Warning: Platform prebuild script should further process and clear $PWD/../data/trik-runtime-builds directory!