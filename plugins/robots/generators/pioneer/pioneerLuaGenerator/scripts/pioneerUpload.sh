# Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
#
# Script for compiling and uploading a program to a quadcopter.
#
# Takes 4 parameters:
# $1 - .lua file (with path) that needs to be compiled and uploaded;
# $2 - connection type (can be "--address" or "--serial");
# $3 - IP address with port, if "--address" is specified as $1, or COM port name if "--serial" is specified;
# $4 - absolute path (with trailing "/") to Lua compiler (leave empty to use compiler from PATH).

LUA_SRC="$1"
LUA_BIN="$1"c

#compile
$4luac5.3 -o $LUA_BIN $LUA_SRC

#upload
BASEDIR=$(dirname "$0")
$BASEDIR/controller.py $2 $3 --write LuaScript:0 -f $LUA_BIN
