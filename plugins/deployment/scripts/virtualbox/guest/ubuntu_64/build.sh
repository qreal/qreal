#/bin/bash

# Usage: build.sh <productId> [version].
# Clones and builds the installers of the given version of TRIK Studio if such was specified and tagged in all remote repositories or builds the version from HEAD of the master branches.
# ssh keys for communication with updates server must be placed in shared folder before this script execution.
# The product name can currently be only "trikStudio" and specifies the builded product name.
# Tag in the remote repositories must be named like "product-id-version" (for example "trikStudio-3.0.0-rc2" where $1 is "trikStudio" and $2 is "3.0.0-rc2").


set -o nounset
set -o errexit

OS=linux-gnu64
OUTPUT_OS=linux-x64
INSTALLER_EXTENSION=.run
QREAL_DIR=/home/qreal/builds
SHARE_DIR=/home/qreal/share
VERSION=${2:-nightly-$(date +'%d-%m-%Y')}
TAG=${2:-}; [ -z "$TAG" ] && : || TAG=$1-$TAG
TARGET=$QREAL_DIR/$VERSION
QT_DIR=/home/qreal/Qt/5.3/gcc_64/bin
QT_IFW_DIR=/home/qreal/QtIFW-1.6.0/bin
QMAKE=$QT_DIR/qmake

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
mkdir -p /home/qreal/.ssh
rm -rf $TARGET
cd $QREAL_DIR
git clone https://github.com/qreal/qreal.git $VERSION
cd $TARGET
function checkout_tag_or_warn { [[ $'\n'$(git tag)$'\n' =~ $'\n'$TAG$'\n' ]] && git checkout $1 || echo "WARNING: TAG  "$TAG" NOT FOUND, BUILDING MASTER"; }
[ -z "${TAG:-}" ] && : || checkout_tag_or_warn $TAG

echo "Building..."
$QMAKE CONFIG+=Release CONFIG-=Debug $PRO_FILE
make -j2
cd installer
[ -f $SHARE_DIR/id_rsa ] && { sudo cp $SHARE_DIR/id_rsa* /home/qreal/.ssh; sudo chmod 777 /home/qreal/.ssh/id_rsa*; } || :
bash build-$IFW_PRODUCT_NAME.sh $QT_DIR $QT_IFW_DIR $TAG

echo "Moving built installers to shared folder..."
function move_installer { sudo mv $IFW_PRODUCT_NAME-$1-$OS-installer $SHARE_DIR/$PRODUCT_DISPLAYED_NAME-$VERSION-$1-$OUTPUT_OS-installer$INSTALLER_EXTENSION; }
move_installer online
move_installer offline
[ -f /home/qreal/.ssh/id_rsa ] && rm /home/qreal/.ssh/id_rsa* || :

echo "Done, you can now collect built installers in the shared folder."
