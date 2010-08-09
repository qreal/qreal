@echo off
set WD=.\source-generator
set GEN=%WD%\generated

cd %WD%
qmake
nmake
del /s /q generated
debug\code.exe

if %ERRORLEVEL% neq 0 (
	echo "Generate-code failed!"
	exit /B 1
)

cd ..\

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
exit  0


