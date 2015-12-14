 #!/bin/bash

# Pass --help option for getting useful information

set -o nounset
set -o errexit

function show_help {
	echo "Usage: update_site_scripts.sh mode version path/to/private/key <key-url mapping with windows offline-online first, linux32 - second and linux64 - last>"
	echo "Example: update_site_scripts.sh latest 3.0.0-rc2 TRIKStudio-3.0.0-rc2-offline-windows-installer https://docs.google.com/file/d/0B-NmiVtuyhyiWkgycTkzWXJ3TVk TRIKStudio-3.0.0-rc2-online-windows-installer https://docs.google.com/file/d/0B-NmiVtuyhyiMEU0WjR0MEp1YkU"
	echo ""
	echo "Clones http://github.com/qreal/tools, edits siteScripts folder contents an pushes back changes to the remote repository and then uploads to production site server."
	echo "The first argument is mode: 'latest' or 'experimental'. In the latest mode all latest references will be updated to windows offline installer. In the exprimental mode latest link will not be replaced but experimental will be."
	echo "Version must be specified in '3.0.0-rc2' format."
	echo "In the tails there must be 2N arguments where N is uploaded installers count. The first argument of each pair is the key of linksMap, second is an installer URL."
	echo "Windows offline installer pair must go first (it will be written into latest and/or experimental sections)."
	echo "Code will be pushed using ~/.ssh/id_rsa. For uploading downloads couter to production server however another ssh key must be used."
	echo "It must be specified as the third parameter. Public key may be stored next to private one and must be named with the same name as private but with .pub suffix."
	echo "~/.ssh/id_rsa and ~/.ssh/id_rsa.pub may be temporaly renamed into ~/.ssh/old_id_rsa and ~/.ssh/old_id_rsa.pub during the uploading process."
	exit 1;
}


[ "$#" -lt 5 ] && show_help || :

echo "Updating site scripts..."

MODE=$1
VERSION=$2
SSH_KEY=$3
TOOLS_DIR=$PWD/temp_tools_clone
DOWNLOADS_COUNTER=$TOOLS_DIR/siteScripts/downloadCounter.html
REMOTE_SERVER=hosting_jzuken@titanium.locum.ru:/home/hosting_jzuken/projects/qreal/htdocs

shift 3

WINDOWS_OFFLINE_LINK=$2
LINUX_32_OFFLINE_LINK=$6
LINUX_64_OFFLINE_LINK=${10}


rm -rf $TOOLS_DIR
git clone git@github.com:qreal/tools.git $TOOLS_DIR
cd $TOOLS_DIR


function create_map {


	RESULT="\n"
	while (( "$#" )); do

		RESULT=$RESULT"\n\ \ \ \ \ \ \ \ \ \ \ \ linksMap[\"$1\"] = \"$2\";"
		shift 2

	done

	echo "$RESULT";

}

function replace_slashes { echo $1 | sed 's/\//\\\//g'; }
function entry_to_find { echo "linksMap\\[\"$1\"\\] = `replace_slashes $2`;"; }
function entry_to_replace { echo "linksMap[\"$1\"] = \"`replace_slashes \"$2\"`"; }
function links_map_pattern { echo "s/`entry_to_find $1 '".*"'`/`entry_to_replace $1 \"$2\";`/g"; }
function main_pattern { links_map_pattern $1 $2'";'; }
function last_pattern { links_map_pattern linux-x64-experimental "$LINUX_64_OFFLINE_LINK\";`echo '\n';create_map $@`"; }
function latest_pattern { main_pattern trikStudio-latest $WINDOWS_OFFLINE_LINK;main_pattern latest $WINDOWS_OFFLINE_LINK;main_pattern linux-x86-latest $LINUX_32_OFFLINE_LINK;main_pattern linux-x64-latest $LINUX_64_OFFLINE_LINK; }
function experimental_pattern { main_pattern experimental $WINDOWS_OFFLINE_LINK;main_pattern linux-x86-experimental $LINUX_32_OFFLINE_LINK;last_pattern $@ ; }

function href_pattern { echo "s/<a href='.*'>/<a href='`replace_slashes $WINDOWS_OFFLINE_LINK`'>/g"; }

if [[ $MODE == "latest" ]]; then
	echo "Replacing all latest links to $WINDOWS_OFFLINE_LINK..."
	SED_PATTERN=`href_pattern;latest_pattern;experimental_pattern $@`
else
	echo "Replacing only experimental link to $WINDOWS_OFFLINE_LINK..."
	SED_PATTERN=`experimental_pattern $@`
fi

sed -i "$SED_PATTERN" $DOWNLOADS_COUNTER

git add $DOWNLOADS_COUNTER
git commit -m "Downloads conter extended with version TRIKStudio-$VERSION"

CURRENT_PRIVATE_KEY=~/.ssh/id_rsa
CURRENT_PUBLIC_KEY=$CURRENT_PRIVATE_KEY.pub
TEMP_PRIVATE_KEY=~/.ssh/old_id_rsa
TEMP_PUBLIC_KEY=$TEMP_PRIVATE_KEY.pub
if [[ -f $CURRENT_PRIVATE_KEY ]]; then
	echo "Temporary renaming $TEMP_PRIVATE_KEY to $CURRENT_PRIVATE_KEY"
	mv $CURRENT_PRIVATE_KEY $TEMP_PRIVATE_KEY
	[ -f $CURRENT_PUBLIC_KEY ] && mv $CURRENT_PUBLIC_KEY $TEMP_PUBLIC_KEY || :
fi
echo "Using $SSH_KEY for uploading to the production server..."
cp $SSH_KEY $CURRENT_PRIVATE_KEY
chmod 700 $CURRENT_PRIVATE_KEY
[ -f $SSH_KEY.pub ] && cp $SSH_KEY.pub $CURRENT_PUBLIC_KEY || :

scp -oStrictHostKeyChecking=no -oUserKnownHostsFile=/dev/null $DOWNLOADS_COUNTER $REMOTE_SERVER

if [[ -f $TEMP_PRIVATE_KEY ]]; then
	echo "Restoring old ssh key"
	mv $TEMP_PRIVATE_KEY $CURRENT_PRIVATE_KEY
	[ -f $TEMP_PUBLIC_KEY ] && mv $TEMP_PUBLIC_KEY $CURRENT_PUBLIC_KEY || :
else
	rm $CURRENT_PRIVATE_KEY
	[ -f $CURRENT_PUBLIC_KEY ] && rm $CURRENT_PUBLIC_KEY || :
fi

echo "Pushing changes to qreal/tools repository..."
git push origin master
echo "Removing temporary files..."
cd ..
rm -rf $TOOLS_DIR
echo "Site scripts updated."
