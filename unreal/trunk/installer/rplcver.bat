@echo off

for /F "tokens=1* delims=]" %%j in ('type "%1" ^| find /V /N ""') do (
	set STRING=%%k
	setlocal EnableDelayedExpansion
	if "!STRING!" == "" ( 
		echo.>> %1.tmp
	) else (
		set STRING=!STRING:0.0.0.0=%2!
		echo !STRING!>> %1.tmp
	)
	endlocal
)

move %1.tmp %1

@echo on