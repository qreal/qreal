#/bin/bash

# Usage: build.sh <productId> [version].
# Clones and builds the installers of the given version of TRIK Studio if such was specified and tagged in all remote repositories or builds the version from HEAD of the master branches.
# ssh keys for communication with updates server must be placed in shared folder before this script execution.
# The product name can currently be only "trikStudio" and specifies the builded product name.
# Tag in the remote repositories must be named like "product-id-version" (for example "trikStudio-3.0.0-rc2" where $1 is "trikStudio" and $2 is "3.0.0-rc2").


set -o nounset
set -o errexit

cd "$(dirname "$0")"

OS=win32
OUTPUT_OS=windows
INSTALLER_EXTENSION=.exe
QREAL_DIR="c:/users/qreal/builds"
SHARE_DIR="\\\\VBOXSVR\\share"
VERSION=${2:-nightly-$(date +'%d-%m-%Y')}
TAG=${2:-}; [ -z "$TAG" ] && : || TAG=$1-$TAG
TARGET=$QREAL_DIR/$VERSION
QT_DIR=c:/Qt/5.3/mingw482_32/bin
QT_IFW_DIR=c:/QtIFW-1.6.0/bin
QMAKE=$QT_DIR/qmake.exe
SSH_DIR=/.ssh
PATH_WITHOUT_BASH="/c/Windows/system32:/c/Windows:/c/Windows/System32/Wbem:/c/Windows/System32/WindowsPowerShell/v1.0/:/c/Qt/5.3/mingw482_32/bin:/c/Qt/Tools/mingw482_32/bin/"
PATH_WITH_BASH="/usr/bin:$PATH_WITHOUT_BASH"

PRODUCT_ID=$1
case "$PRODUCT_ID" in
"trikStudio")
	PRODUCT_DISPLAYED_NAME="TRIKStudio"
	PRO_FILE="qrealRobots.pro"
	IFW_PRODUCT_NAME="trik-studio"
	;;
*)
	>&2 echo "Unknown product name $1. Supported options are currently just 'trikStudio'"
	exit 1
	;;
esac


[ -f $SHARE_DIR/id_rsa ] \
	&& echo "WARNING: id_rsa found in shared folder. This means that this version build will be automaticly uploaded to remote autoupdate server." \
	|| echo "WARNING: id_rsa not found in shared folder. This means that this version will NOT be automaticly uploaded to remote autoupdate server."

mkdir -p $QREAL_DIR
mkdir -p $SSH_DIR

#rm -rf $TARGET
#cd $QREAL_DIR
#git clone https://github.com/qreal/qreal.git $VERSION
cd $TARGET
function checkout_tag_or_warn { { PATTERN=$'\n'$1$'\n'; echo "$'\n'$(git tag)$'\n'" | grep -E $PATTERN; } && git checkout $1 || echo "WARNING: TAG  "$TAG" NOT FOUND, BUILDING MASTER"; }
[ -z "${TAG:-}" ] && : || checkout_tag_or_warn $TAG

#echo "Building..."
#export PATH=$PATH_WITHOUT_BASH
#$QMAKE CONFIG+=Release CONFIG-=Debug $PRO_FILE
#C:/Qt/Tools/mingw482_32/bin/mingw32-make -j2
#export PATH=$PATH_WITH_BASH
cd installer
[ -f $SHARE_DIR/id_rsa ] && cp $SHARE_DIR/id_rsa $SSH_DIR || :
[ -f $SHARE_DIR/id_rsa.pub ] && cp $SHARE_DIR/id_rsa.pub $SSH_DIR || :
bash build-$IFW_PRODUCT_NAME.sh $QT_DIR $QT_IFW_DIR $TAG

echo "Moving built installers to shared folder..."
function move_installer { mv $IFW_PRODUCT_NAME-$1-$OS-installer$INSTALLER_EXTENSION $SHARE_DIR/$PRODUCT_DISPLAYED_NAME-$VERSION-$1-$OUTPUT_OS-installer$INSTALLER_EXTENSION; }
move_installer online
move_installer offline
[ -f $SSH_DIR/id_rsa ] && rm $SSH_DIR/id_rsa || :
[ -f $SSH_DIR/id_rsa.pub ] && rm $SSH_DIR/id_rsa.pub || :

echo "Done, you can now collect built installers in the shared folder."
