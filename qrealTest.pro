include (qreal.pri)

CONFIG += c++11

SUBDIRS += \
	qrtest \

qrtest.depends = \
	qrmc \
	qrxc \
	qrgui \
	qrrepo \
	plugins \
	qrkernel \
	qrutils \
	thirdparty \

