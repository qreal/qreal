#!/bin/bash
set -o nounset
set -o errexit

# Walks through the object`s dependencies and replaces dirname of matching with new prefix.
function fix_dependencies {

	export OBJECT=$1 # Lib or executable whoose dependencies we fix
	export PREFIX=$2 # A path to match replaced dependencies, for example ~/Qt/5.4/clang_64/lib
	export TARGET=$3 # A path that will be used to rewrite dependencies, for example @executable_path/../../../ 

	# Normalizing prefix path...
	OLD_PATH=$PWD
	cd $PREFIX
	PREFIX=`pwd`
	cd $OLD_PATH

	# For each lib in $PREFIX replacing path to it with $TARGET
	otool -L "$OBJECT" | grep -Go "$PREFIX[A-Za-z_0-9/\.]*" | xargs -I {} bash -c 'install_name_tool -change {} $TARGET/`basename {}`.dylib "$OBJECT"' _
}

function copy_qt_lib {
	cp -pr $QT_DIR/lib/$1.framework/Versions/5/$1 $PWD/../data/$1.dylib
	fix_dependencies $PWD/../data/$1.dylib $QT_DIR/lib @executable_path/../../..
}
