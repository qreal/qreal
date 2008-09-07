@echo off
rem clean.bat
start /wait generate-code-vs2005.bat
qmake 
nmake 
cd reposerver 
qmake 
nmake
