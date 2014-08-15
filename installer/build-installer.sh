#!/bin/bash
set -o nounset
set -o errexit

# Usage: build-installer.sh <path-to-qt> <product-name>
# Path to Qt should look like ~/Qt/5.3/gcc_64/ (bin and lib directories must be in the specified folder).
# This will build the installer using the config in './config/$2-config.xml' with 'qrgui' renamed to $2.
# Qt Installer Framework must be installer into the Qt directory. For example if Qt libs are in ~/Qt/5.3/gcc_64/lib
# then Qt Installer Framework must be in ~/Qt/QtIFW-*/bin


export INSTALLER_ROOT=$PWD/
export BIN_DIR=$PWD/../bin
export QT_DIR=$1
export PRODUCT=$2

# $2 will be passed to all prebuild.sh scripts
echo "Executing prebuild actions..."
find $PWD -name prebuild-common.sh -print0 | xargs -0 chmod +x
find $PWD -name prebuild-$OSTYPE.sh -print0 | xargs -0 chmod +x
find $PWD -name prebuild-common.sh | bash
find $PWD -name prebuild-$OSTYPE.sh | bash

echo "Building installer..."
$QT_DIR/../../QtIFW-*/bin/binarycreator -c config/$PRODUCT-config.xml -p packages/qreal-base -p packages/$PRODUCT $PRODUCT-installer

echo "Done"
