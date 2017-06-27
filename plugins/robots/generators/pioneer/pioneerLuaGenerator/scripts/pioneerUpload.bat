set LUA_SRC=%1
set LUA_BIN=%1c

%4luac -o "%LUA_BIN%" "%LUA_SRC%"

%5 %6controller.py %2 %3 --write LuaScript:0 -f "%LUA_BIN%"
