#!/bin/sh
slice2cpp  repoclientice.ice
slice2cs  repoclientice.ice
mv -f repoclientice.cs client
