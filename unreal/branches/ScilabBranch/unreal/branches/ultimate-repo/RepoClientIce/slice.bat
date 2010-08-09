rem @echo off
set LANG=C
slice2cpp  repoclientice.ice
slice2cs  repoclientice.ice
copy /Y repoclientice.cs client\
copy /Y repoclientice.cpp server\
copy /Y repoclientice.h server\
rem del /Q repoclient.cs
