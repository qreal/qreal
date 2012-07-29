include (qreal.pri)

SUBDIRS += qrtest

qrgui.file = qrgui/qrguiTest.pro
qrxc.file = qrxc/qrxcTest.pro

qrtest.depends = \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrmc \
	qrgui \
