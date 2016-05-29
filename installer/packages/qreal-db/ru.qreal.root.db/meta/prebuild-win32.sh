#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/tools/databasesSupport.dll 	$PWD/../data/plugins/tools/
cp    $BIN_DIR/plugins/editors/databasesEditor.dll 	$PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/microsoftAccess.dll 	$PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/mySql5.dll 			$PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/postgreSql.dll		$PWD/../data/plugins/editors
cp    $BIN_DIR/plugins/editors/sqlite.dll			$PWD/../data/plugins/editors						
cp    $BIN_DIR/plugins/editors/sqlServer2008.dll 	$PWD/../data/plugins/editors

cp    $QT_DIR/bin/Qt5Sql.dll						$PWD/../data

cp    $QT_DIR/plugins/sqldrivers/qsqlite.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlited.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlmysql.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlmysqld.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlodbc.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlodbcd.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlpsql.dll		$PWD/../data/sqldrivers/
cp    $QT_DIR/plugins/sqldrivers/qsqlpsqld.dll		$PWD/../data/sqldrivers/