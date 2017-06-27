#!/bin/bash
#
# Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
#
# Script that is used by Pioneer HTTP communicator to compile a program before uploading it to a robot.
# Takes 2 parameters:
# $1 - file name (with path) of a .lua program to compile;
# $2 - absolute path to Lua compiler that shall be used (leave it blank to use compiler from PATH).

$2luac5.3 -o "$1" "$1c"

