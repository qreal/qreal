#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/rulesChecker.dll                        $PWD/../data/plugins/tools/
cp    $BIN_DIR/plugins/tools/updatesChecker.dll                      $PWD/../data/plugins/tools/

cp    $BIN_DIR/plugins/editors/bpmn.dll                              $PWD/../data/plugins/editors/
cp    $BIN_DIR/plugins/editors/classDiagram.dll                      $PWD/../data/plugins/editors/
