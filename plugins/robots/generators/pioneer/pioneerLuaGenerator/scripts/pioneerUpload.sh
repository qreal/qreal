#!/bin/bash
#
# Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
#

LUA_SRC="$1"
LUA_BIN="$1"c

#compile
$3luac5.3 -o $LUA_BIN $LUA_SRC

#upload
$5 $6controller.py $2 $3 --write LuaScript:0 -f $LUA_BIN

# curl -X POST -T "$1" -I http://$2:8080/pioneer/v0.1/upload
