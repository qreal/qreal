pushd
pushd
echo Generating editors...
call generate-code.bat
popd
echo Building QReal...
qmake 
mingw32-make
echo Building reposerver...
cd reposerver 
qmake 
mingw32-make
popd
