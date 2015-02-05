TEMPLATE = subdirs

include(../../../../global.pri)

copyToDestdir(help/ changelog.txt, now)

OTHER_FILES += \
	doc/changelog.txt \
	doc/help/contents.html.js \
	doc/help/ev3.html \
	doc/help/footer.html.js \
	doc/help/header.html.js \
	doc/help/index.html \
	doc/help/interface.html \
	doc/help/nxt.html \
	doc/help/programming.html \
	doc/help/run-programs.html \
	doc/help/style.css \
	doc/help/trik.html \
	doc/help/version.js \

#win32 {
#	DESTDIR ~= s,/,\,g
#	system("cmd /C "xcopy html $$DESTDIR\\help\\ /s /e /q /y /i &&"\
#			" copy changelog.txt $$DESTDIR /y"\
#			"\
#	)
#} else {
#	system(" mkdir $$DESTDIR/help/;"\
#			" cp -r ../doc/html/* $$DESTDIR/help/ &&"\
#			" cp ../doc/changelog.txt $$DESTDIR"\
#	)
#}
