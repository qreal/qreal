 #!/bin/bash

# Pass --help option for getting useful information

set -o nounset
set -o errexit

function show_help {
	echo "Usage: replace.sh <old-version> <new-version> <where/to/replace> [files filter regexp]"
	echo "Example: replace.sh 3.0.0-2-1 3.0.0-3-1 ~/qreal/installer/ \"*/meta/package.xml\""
	echo ""
	echo "Replaces all the occurenses of the first argument value with the second argument value."
	echo "Third argument specifies a place where to perform replacement and may contain wildcards (but must be specified in quetes then!)."
	echo "Filtering regexp can be specified with the last argument by it is optional."
	exit 0;
}


[ "$#" -lt 3 ] && show_help || :

[ "$#" -ge 4  ] && FILTER=-path \"$4\" || FILTER=

echo "Replacing $1 to $2 in $3..."
find $3 $FILTER -type f -print0 | xargs -0 sed -i "{N; s/$1/$2/g}"
