@echo off
slice2cpp  repoclientice.ice
slice2cs  repoclientice.ice
copy repoclientice.cs client
del repoclient.cs
