@echo off
rem clean.bat
start /wait generate-code-vs2005.bat
if %ERRORLEVEL% neq 0 ( 
	echo "Generate-code failed, aborting."
	exit /B 1
)
qmake 
nmake 
cd reposerver 
qmake 
nmake
