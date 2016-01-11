#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

# Cleaning up prebuild-common.sh results...
mkdir -p "$BUNDLE_CONTENTS/Resources"
mv $PWD/../data/translations "$BUNDLE_CONTENTS/Resources"
