@rem Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
@rem
@rem Script for compiling and uploading a program to a quadcopter.
@rem
@rem Takes 4 parameters:
@rem %1 - .lua file (with path) that needs to be compiled and uploaded;
@rem %2 - connection type (can be "--address" or "--serial");
@rem %3 - IP address with port, if "--address" is specified as $1, or COM port name if "--serial" is specified;
@rem %4 - absolute path (with trailing "/") to Lua compiler (leave empty to use compiler from PATH).

set LUA_SRC=%1
set LUA_BIN=%1c

%4luac -o "%LUA_BIN%" "%LUA_SRC%"

%~dp0controller.exe %2 %3 --write LuaScript:0 -f "%LUA_BIN%"
