#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-interpreter.dll               $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-qts-generator.dll             $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-trik-runtime-uploader-plugin.dll   $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/robots-trik-generator-base.dll                                     $PWD/../data/


cp    $QT_DIR/bin/Qt5Network.dll                                                  $PWD/../data

function checkout_tag_or_warn { { PATTERN=$'\n'$1$'\n'; echo "$'\n'$(git tag)$'\n'" | grep -E $PATTERN; } && git checkout $1 || echo "Warning: TRIKRuntime doesn't have tag "$1; }

cd $PWD/../data/
rm -rf trik-runtime-builds
git clone  --depth 1 https://github.com/qreal/trik-runtime-builds

[ -z "${QREAL_BUILD_TAG:-}" ] && : || { cd trik-runtime-builds; checkout_tag_or_warn $QREAL_BUILD_TAG; }
cd "$(dirname "$0")"/../data
rm -rf winscp
mv trik-runtime-builds/winscp $PWD
mkdir -p trikRuntime
cd trikRuntime
tar xfvzh ../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../trik-runtime-builds
