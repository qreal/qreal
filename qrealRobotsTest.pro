include (qreal.pri)

plugins.file = plugins/robots.pro

SUBDIRS += \
	gmock \
	robotsTests \

gmock.file = qrtest/thirdparty/gmock-1.6.0/gmock.pro
robotsTests.file = qrtest/unitTests/pluginsTests/robotsTests/robotsTests.pro

robotsTests.depends = \
	qrxc \
	qrgui \
	qrrepo \
	plugins \
	qrkernel \
	qrutils \
	thirdparty \

