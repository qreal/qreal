include(../../../../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/unittests/

QT += widgets

links(robots-interpreter-base)

INCLUDEPATH += \
	../../../../../../../plugins/robots/interpreters/interpreterBase/include/ \
	../../../../../../../plugins/robots/utils/include/ \
	../../../../../../../qrgui/ \
	../../../../../../../ \
	../../../../../../../qrtext/include \

HEADERS += \
	mockKitPlugin2Interface.h \

SOURCES += \
	mockKitPlugin2Interface.cpp \
