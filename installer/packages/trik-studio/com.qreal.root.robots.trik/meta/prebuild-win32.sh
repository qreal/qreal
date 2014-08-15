#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/plugins/tools/kitPlugins/trikKitInterpreter.dll       $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/trikGenerator.dll            $PWD/../data/plugins/tools/kitPlugins/


cp    $QT_DIR/bin/Qt5Network.dll                                     $PWD/../data
