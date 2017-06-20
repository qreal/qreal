set LUA_SRC=%1
set LUA_BIN=%1c

luac -o "%LUA_BIN%" "%LUA_SRC%"

python controller.py --address %2:8000 --write LuaScript:0 -f "%LUA_BIN%"

rem curl -X POST -T "$1" -I http://$2:8080/pioneer/upload
