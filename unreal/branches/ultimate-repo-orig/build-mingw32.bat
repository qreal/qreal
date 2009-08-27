pushd
pushd
echo Generating editors...
call generate-code.bat
if %ERRORLEVEL% neq 0 ( 
	echo "Generate-code failed, aborting."
	exit /B 1
)
popd
echo Building QReal...
qmake 
mingw32-make -j2
echo Building reposerver...
cd reposerver 
qmake 
mingw32-make -j2
popd
