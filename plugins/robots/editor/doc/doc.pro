TEMPLATE = subdirs

include(../../../../global.pri)

copyToDestdir(help/ changelog.txt, now)

OTHER_FILES += \
	changelog.txt \
	help/contents.html.js \
	help/ev3.html \
	help/footer.html.js \
	help/header.html.js \
	help/index.html \
	help/interface.html \
	help/nxt.html \
	help/programming.html \
	help/run-programs.html \
	help/style.css \
	help/trik.html \
	help/version.js \
