#!/bin/bash
# Usage: build-installer.sh <path-to-qt> <path-to-qt-ifw> <product-name> [<additional builder args>]
# Path to Qt and Qt Installer Framework must be to bin folder (for example ~/Qt/5.3/gcc_64/bin or ~/Qt/QtIFW-1.5.0/bin/).
# This script will build the installer using the config in './config/$3-config.xml',
# and all components in 'packages/qreal-base' and 'packages/$3'. 'qrgui' will be renamed to $3.

set -o nounset
set -o errexit

export INSTALLER_ROOT=$PWD/
export BIN_DIR=$PWD/../bin
export QT_DIR=$1/../
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

echo "Building online installer..."
$QTIFW_DIR/binarycreator --online-only -c config/$PRODUCT-$OS.xml -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-online-$OS-installer

echo "Building offline installer..."
$QTIFW_DIR/binarycreator --offline-only -c config/$PRODUCT-$OS.xml -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-offline-$OS-installer

echo "Building updates repository..."
$QTIFW_DIR/repogen -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-repository

echo "Uploading repository to server..."
scp -r $PRODUCT-repository/* qrealproject@195.19.241.150:/home/qrealproject/public/$PRODUCT-repo-$OS

echo "Removing temporary files..."
rm -rf $PRODUCT-repository

echo "Done"
