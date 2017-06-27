set LUA_SRC=%1
set LUA_BIN=%1c

%3luac -o "%LUA_BIN%" "%LUA_SRC%"

%4 %5controller.py --address %2:8000 --write LuaScript:0 -f "%LUA_BIN%"
