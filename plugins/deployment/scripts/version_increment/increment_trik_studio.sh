 #!/bin/bash

# Pass --help option for getting useful information

set -o nounset
set -o errexit

cd "$(dirname "$0")"

function show_help {
	echo "Usage: increment_trik_studio.sh <path-to-qreal> <old-version> <new-version>"
	echo "Example: increment_trik_studio.sh ~/qreal 3.0.0-b2 3.0.0-rc1"
	echo ""
	echo "Extends TRIKStudio version in the source code, commits and pushes changes."
	echo "Versions must be specified in short format (3.0.0-a1 instead of 3.0.0-alpha1)"
	exit 0;
}

[ "$#" -eq 3 ] && : || show_help

ROOT=$1
SHORT_OLD_VERSION=$2
SHORT_NEW_VERSION=$3
LONG_OLD_VERSION=`echo $SHORT_OLD_VERSION | sed "s/-a/-alpha/g;s/-b/-beta/g"`
LONG_NEW_VERSION=`echo $SHORT_NEW_VERSION | sed "s/-a/-alpha/g;s/-b/-beta/g"`
GREEK_OLD_VERSION=`echo $SHORT_OLD_VERSION | sed "s/-a/-α/g;s/-b/-β/g"`
GREEK_NEW_VERSION=`echo $SHORT_NEW_VERSION | sed "s/-a/-α/g;s/-b/-β/g"`
DIGITAL_OLD_VERSION=`echo $SHORT_OLD_VERSION | sed "s/-a/-1-/g;s/-b/-2-/g;s/-rc/-3-/g"`
DIGITAL_NEW_VERSION=`echo $SHORT_NEW_VERSION | sed "s/-a/-1-/g;s/-b/-2-/g;s/-rc/-3-/g"`
function add_suffix { [[ $1 =~ "-" ]] && echo $1 || echo $1-4; }
DIGITAL_OLD_VERSION=`add_suffix $DIGITAL_OLD_VERSION`
DIGITAL_NEW_VERSION=`add_suffix $DIGITAL_NEW_VERSION`

DATE="$(date +'%Y-%m-%d')"
YEAR="$(date +'%Y')"

bash $PWD/replace.sh "version=\"$SHORT_OLD_VERSION\"" "version=\"$SHORT_NEW_VERSION\"" $ROOT/plugins/robots/editor/generated/robotsMetamodel.xml
bash $PWD/replace.sh "var version = \".*\";" "var version = \"$GREEK_NEW_VERSION\";" $ROOT/plugins/robots/editor/doc/help/version.js
bash $PWD/replace.sh "var year = [0-9]\\+;" "var year = $YEAR;" $ROOT/plugins/robots/editor/doc/help/version.js
bash $PWD/replace.sh "return \"$GREEK_OLD_VERSION\";" "return \"$GREEK_NEW_VERSION\";" $ROOT/plugins/robots/interpreters/interpreterCore/src/customizer.cpp
bash $PWD/replace.sh "const QString requiredVersion = \"$SHORT_OLD_VERSION\"" "const QString requiredVersion = \"$SHORT_NEW_VERSION\"" $ROOT/plugins/robots/utils/include/utils/requiredVersion.h
bash $PWD/replace.sh "<Version>$GREEK_OLD_VERSION<\\/Version>" "<Version>$GREEK_NEW_VERSION<\\/Version>" $ROOT/installer/config "trik-studio*.xml"
bash $PWD/replace.sh "<Version>$DIGITAL_OLD_VERSION<\\/Version>\n    <ReleaseDate>.*<\\/ReleaseDate>" "<Version>$DIGITAL_NEW_VERSION<\\/Version>\\n    <ReleaseDate>$DATE<\\/ReleaseDate>" $ROOT/installer/packages "*/meta/package.xml"

cd $ROOT
git add -A && git commit -m "TRIK Studio version incremented to $SHORT_NEW_VERSION"
git tag trikStudio-$SHORT_NEW_VERSION
git push origin master
git push origin trikStudio-$SHORT_NEW_VERSION
