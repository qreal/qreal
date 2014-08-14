#!/bin/bash
set -o nounset
set -o errexit

# Usage: build-linux <path-to-qt> <config-file>
# This will build linux installer using the config in ./config/$2-config.xml with qrgui renamed to $2.
# Qt Installer Framework must be installer into the Qt directory. For example if Qt libs are in ~/Qt/5.3/gcc_64/lib
# then Qt Installer Framework must be in ~/Qt/QtIFW-*/bin

export INSTALLER_ROOT=$PWD/
export BIN_DIR=$PWD/../bin
export QT_DIR=$1

echo "Executing prebuild actions..."
# $2 will be passed to all prebuild.sh scripts
find $PWD -name prebuild.sh -print0 | xargs -0 chmod +x
find $PWD -name prebuild.sh         | xargs -i bash {} $2

echo "Building installer..."
$QT_DIR/../../QtIFW-*/bin/binarycreator -c config/$2-config.xml -p packages $2-installer

echo "Done"
