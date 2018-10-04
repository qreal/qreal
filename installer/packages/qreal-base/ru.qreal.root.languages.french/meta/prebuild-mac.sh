#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

# Cleaning up prebuild-common.sh results...

TARGET_DIR="$BUNDLE_CONTENTS/Resources"
rm -rf "$TARGET_DIR" && mkdir -p "$TARGET_DIR"
mv "$PWD/../data/translations" "$TARGET_DIR"
