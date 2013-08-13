include (qreal.pri)

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

