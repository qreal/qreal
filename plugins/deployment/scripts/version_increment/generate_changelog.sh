 #!/bin/bash

# Pass --help option for getting useful information

set -o nounset
set -o errexit

cd "$(dirname "$0")"

function show_help {
	echo "Usage: generate_changelog.sh <path-to-qreal> <source-tag>"
	echo "Example: generate_changelog.sh ~/qreal trikStudio-3.0.0-rc2"
	echo ""
	echo "Prepends the TRIK Studio changelog file with commit messages starting from the given tag."
	exit 0;
}

[ "$#" -eq 2 ] && : || show_help

ROOT=$1
TAG=$2
CHANGELOG=$ROOT/plugins/robots/editor/doc/changelog.txt
TEMP=$ROOT/plugins/robots/editor/doc/changelog_temp.txt

cd $ROOT
function errcho() { echo $1; exit 1; }
[[ $'\n'$(git tag)$'\n' =~ $'\n'$TAG$'\n' ]] && : || errcho "Specified path doesn't have tag $TAG"

TAG_HASH=`git rev-list $TAG | head -n 1`

echo "TRIKStudio $TAG:" | sed "s/trikStudio-//g" >$TEMP
git log --pretty="format:%an: %s" $TAG_HASH..HEAD >>$TEMP
echo "" >>$TEMP
echo "" >>$TEMP
cat $CHANGELOG >>$TEMP
cat $TEMP >$CHANGELOG
rm $TEMP
