include (qreal.pri)

plugins.file = $$PWD/plugins/robots.pro

SUBDIRS += \
	gmock \
	robotsTests \

gmock.file = $$PWD/qrtest/thirdparty/gmock-1.6.0/gmock.pro
robotsTests.subdir = $$PWD/qrtest/unitTests/pluginsTests/robotsTests

robotsTests.depends = \
	qrxc \
	qrgui \
	qrrepo \
	plugins \
	qrkernel \
	qrutils \
	thirdparty \

