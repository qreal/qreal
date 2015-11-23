#!/bin/bash
set -o nounset
set -o errexit

export BUNDLE_CONTENTS=$PWD/../data/$PRODUCT_DISPLAYED_NAME.app/Contents/
export LIB_PATH=@executable_path/../Lib

# Walks through the object`s dependencies and replaces dirname of matching with new prefix.
function fix_dependencies {

	export OBJECT="$1" # Lib or executable whoose dependencies we fix
	export PREFIX="$2" # A path to match replaced dependencies, for example ~/Qt/5.4/clang_64/lib
	export TARGET="$3" # A path that will be used to rewrite dependencies, for example @executable_path/../../../ 

	# Normalizing prefix path...
	OLD_PATH=$PWD
	cd $PREFIX 2>/dev/null && PREFIX=`pwd` || :
	cd $OLD_PATH

	# For each lib in $PREFIX replacing path to it with $TARGET
	otool -L "$OBJECT" | grep -Go "$PREFIX[A-Za-z_0-9/\.-]*" | xargs -I {} bash -c 'install_name_tool -change {} $TARGET/`basename {}` "$OBJECT"' _
}

function fix_qreal_dependencies {
	fix_dependencies "$1" "$QT_DIR" "$LIB_PATH"
	fix_dependencies "$1" "@rpath/.*\.framework/Versions/5/" "$LIB_PATH"
	fix_dependencies "$1" "@executable_path/../../.." "$LIB_PATH"
}

function copy_qt_lib {
	cp -R "$QT_DIR/lib/$1.framework/Versions/5/$1" "$BUNDLE_CONTENTS/Lib/$1"
	fix_dependencies "$BUNDLE_CONTENTS/Lib/$1" "$QT_DIR/lib" "$LIB_PATH"
}
