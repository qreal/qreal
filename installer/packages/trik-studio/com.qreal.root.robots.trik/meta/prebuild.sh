#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


mkdir -p $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/libtrikKitInterpreter.so       $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/libtrikGenerator.so            $PWD/../data/plugins/tools/kitPlugins/


cp -pr $QT_DIR/lib/libQt5Network.so*                                    $PWD/../data
