#!/bin/bash
# Usage: build-installer.sh <path-to-qt> <path-to-qt-ifw> <product-name> [<additional builder args>]
# Path to Qt and Qt Installer Framework must be to bin folder (for example ~/Qt/5.3/gcc_64/bin or ~/Qt/QtIFW-1.5.0/bin/).
# This script will build the installer using the config in './config/$3-config.xml',
# and all components in 'packages/qreal-base' and 'packages/$3'. 'qreal' will be renamed to $3.
# If QREAL_BUILD_TAG is nonempty then the version will be built using dependencies tagged with its value.

set -o nounset
set -o errexit

export INSTALLER_ROOT=$PWD/
export BIN_DIR=$PWD/../bin/release/
export QT_DIR=$1/../
export QTIFW_DIR=$2
export PRODUCT=$3 
export OS=$OSTYPE

grep -q "darwin" <<< $OSTYPE && export OS="mac" || :
# All windows platforms can be enumerated below
[ $OSTYPE == "msys" ] && export OS="win32" || :
[ $OSTYPE == "linux-gnu" ] && OS_EXT=$OS`getconf LONG_BIT` || OS_EXT=$OS

[ $OS == "win32" ] && SSH_DIR=/.ssh || SSH_DIR=~/.ssh

# $2 will be passed to all prebuild.sh scripts
echo "Executing prebuild actions..."
find $PWD/packages/qreal-base -name prebuild-common.sh -print0 | xargs -0 chmod +x
find $PWD/packages/qreal-base -name prebuild-$OS.sh -print0 | xargs -0 chmod +x
find $PWD/packages/qreal-base -name prebuild-common.sh | bash
find $PWD/packages/qreal-base -name prebuild-$OS.sh | bash

find $PWD/packages/$PRODUCT -name prebuild-common.sh -print0 | xargs -0 chmod +x
find $PWD/packages/$PRODUCT -name prebuild-$OS.sh -print0 | xargs -0 chmod +x
find $PWD/packages/$PRODUCT -name prebuild-common.sh | bash
find $PWD/packages/$PRODUCT -name prebuild-$OS.sh | bash

echo "Building online installer..."
$QTIFW_DIR/binarycreator --online-only -c config/$PRODUCT-$OS_EXT.xml -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-online-$OS_EXT-installer

echo "Building offline installer..."
$QTIFW_DIR/binarycreator --offline-only -c config/$PRODUCT-$OS_EXT.xml -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-offline-$OS_EXT-installer

[ -f $SSH_DIR/id_rsa ] && : || { echo "Done"; exit 0; }

echo "Building updates repository... This step can be safely skipped, the offline installer is already ready, press Ctrl+C if you are not sure what to do next."
rm -rf $PRODUCT-repository
$QTIFW_DIR/repogen -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-repository

echo "Uploading repository to server... This step can be also safely skipped, the offine installer is already ready, press Ctrl+C if you are not sure what to do next."
scp -oStrictHostKeyChecking=no -oUserKnownHostsFile=/dev/null -r $PRODUCT-repository/* qrealproject@195.19.241.150:/home/qrealproject/public/packages/$PRODUCT-repo-$OS_EXT

echo "Removing temporary files..."
rm -rf $PRODUCT-repository

echo "Done"
