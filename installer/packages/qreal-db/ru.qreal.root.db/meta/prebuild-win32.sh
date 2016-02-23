#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/databasesSupport.dll $PWD/../data/plugins/tools/
cp    $BIN_DIR/plugins/editors/databasesEditor.dll $PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/microsoftAccess.dll $PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/mySql5.dll $PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/postgreSql.dll $PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/sqlite.dll $PWD/../data/plugins/editors						
cp    $BIN_DIR/plugins/editors/sqlServer2008.dll $PWD/../data/plugins/editors