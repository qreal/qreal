TEMPLATE = subdirs

win32 {
	system(cmd /C "xcopy *.qm ..\\bin\\translations\\ /s /e /y")
}
else {
	system(mkdir -p ../bin/translations/; find ./ -name *.qm -exec cp --parents {} ../bin/translations \;)
}
