include (qreal.pri)

SUBDIRS += \
	qrtest \
	qrxcTest \
	qrguiTest \

qrtest.depends = \
	thirdparty \

qrxcTest.file = qrxc/qrxcTest.pro
qrguiTest.file = qrgui/qrguiTest.pro

qrxcTest.depends = qrxc
qrguiTest.depends = qrgui
