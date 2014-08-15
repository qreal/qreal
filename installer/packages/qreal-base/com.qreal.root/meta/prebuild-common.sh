#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $INSTALLER_ROOT/licenses/gpl-3.0*.txt                             $PWD
cp     $INSTALLER_ROOT/licenses/OFL*.txt                                 $PWD
cp     $INSTALLER_ROOT/licenses/QsKineticScroller-license*.txt           $PWD
cp     $INSTALLER_ROOT/licenses/QsLog-license*.txt                       $PWD
cp     $INSTALLER_ROOT/licenses/QtPropertyBrowser-license*.txt           $PWD
cp     $INSTALLER_ROOT/licenses/Scintilla-license*.txt                   $PWD
cp     $INSTALLER_ROOT/licenses/WindowsModernStyle-license*.txt          $PWD


mkdir -p $PWD/../data/platforms
mkdir -p $PWD/../data/imageformats
mkdir -p $PWD/../data/iconengines
cp     $BIN_DIR/changelog.txt                                            $PWD/../data
