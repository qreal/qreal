#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp    $BIN_DIR/plugins/tools/kitPlugins/robots-nxt-interpreter.dll          $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-nxt-osek-c-generator.dll     $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/plugins/tools/kitPlugins/robots-nxt-russian-c-generator.dll  $PWD/../data/plugins/tools/kitPlugins/
cp    $BIN_DIR/robots-nxt-generator-base.dll                                $PWD/../data/
cp    $BIN_DIR/robots-nxt-kit.dll                                           $PWD/../data/
cp    $BIN_DIR/qextserialport.dll                                           $PWD/../data/
