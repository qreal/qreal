#!/bin/bash
#
# Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
#
# Script for executing a program on a quadcopter.
#
# Takes 3 arguments:
# $1 - connection type (can be "--address" or "--serial");
# $2 - IP address with port, if "--address" is specified as $1, or COM port name if "--serial" is specified.
# $3 - command to execute (can be "--runLuaScript" or "--stopLuaScript")

BASEDIR=$(dirname "$0")
$BASEDIR/controller $1 $2 $3
