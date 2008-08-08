#!/bin/sh
export PATH=$PATH:/opt/Ice-3.3/bin/
slice2cpp  repoclientice.ice
slice2cs  repoclientice.ice
mv -f repoclientice.cs client
