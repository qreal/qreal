@echo off
set WD=..\source-generator
set GEN=%WD%\generated

cd %WD%
qmake
mingw32-make
del /s /q generated
release\code.exe
cd ..\ultimate-repo

md umllib\generated
xcopy /y /e /c /h /r %GEN%\umllib\generated\* umllib\generated

xcopy /y /e /c /h /r %GEN%\umllib\uml_guiobjectfactory.cpp umllib

xcopy /y /e /c /h /r %GEN%\repo\* repo
md reposerver\generated
xcopy /y /e /c /h /r %GEN%\reposerver\* reposerver

del /s /q shapes

xcopy /y /i /e /c /h /r %GEN%\shapes shapes

xcopy /y /e /c /h /r %GEN%\real_dynamic.qrc .
:q
echo "done"
