# Here must be the call to QRMC console generator

TARGET = generated

TEMPLATE = subdirs

SUBDIRS = \
	generated \

generated.file = generated/robotsMetamodel.pro

OTHER_FILES += \
	doc/changelog.txt \
	doc/html/contents.html.js \
	doc/html/ev3.html \
	doc/html/footer.html.js \
	doc/html/header.html.js \
	doc/html/index.html \
	doc/html/interface.html \
	doc/html/nxt.html \
	doc/html/programming.html \
	doc/html/run-programs.html \
	doc/html/style.css \
	doc/html/trik.html \
	doc/html/version.js \
