TEMPLATE = subdirs

include(../global.pri)


win32 {
	DESTDIR ~= s,/,\,g
	system(cmd /C "xcopy *.qm $$DESTDIR\\translations\\ /s /e /y")
}
else {
	system(mkdir -p $$DESTDIR/translations/; find ./ -name *.qm -exec cp --parents {} $$DESTDIR/translations \;)
}
