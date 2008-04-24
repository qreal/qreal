all: repoclientice.ice
	slice2cpp  $^
	slice2cs  $^
	/bin/mv -f repoclientice.cpp repoclientice.h server
	/bin/mv -f repoclientice.cs client
