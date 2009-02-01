@echo off
set LANG=C
slice2cpp  repoclientice.ice
slice2cs  repoclientice.ice
rem copy repoclientice.cs client
rem del /q repoclient.cs
