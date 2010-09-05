@echo off
setlocal EnableDelayedExpansion

for /F "tokens=* delims==" %%A in (%1) do (
	set STRING=%%A
	set STRING=!STRING:0.0.0.0=%2!
	echo !STRING!>> %1.tmp
)

move %1.tmp %1