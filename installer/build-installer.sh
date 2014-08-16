#!/bin/bash
set -o nounset
set -o errexit

# Usage: build-installer.sh <path-to-qt> <path-to-qt-ifw> <product-name> [<additional builder args>]
# Path to Qt should look like ~/Qt/5.3/gcc_64/ (bin and lib directories must be in the specified folder).
# Path to Qt Installer Framework should look like ~/Qt/QtIFW-1.5.0/bin/ (must be to bin folder).
# This script will build the installer using the config in './config/$3-config.xml',
# and all components in 'packages/qreal-base' and 'packages/$3'. 'qrgui' will be renamed to $3.


export INSTALLER_ROOT=$PWD/
export BIN_DIR=$PWD/../bin
export QT_DIR=$1
export QTIFW_DIR=$2
export PRODUCT=$3 
export OS=$OSTYPE
# All windows platforms can be enumerated below
[ $OSTYPE == "msys" ] && export OS="win32"

# $2 will be passed to all prebuild.sh scripts
echo "Executing prebuild actions..."
find $PWD -name prebuild-common.sh -print0 | xargs -0 chmod +x
find $PWD -name prebuild-$OS.sh -print0 | xargs -0 chmod +x
find $PWD -name prebuild-common.sh | bash
find $PWD -name prebuild-$OS.sh | bash

echo "Building installer..."
$QTIFW_DIR/binarycreator -c config/$PRODUCT-config.xml -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-installer

echo "Done"
