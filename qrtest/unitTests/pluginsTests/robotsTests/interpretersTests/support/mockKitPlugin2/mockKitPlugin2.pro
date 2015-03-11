include(../../../../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/unittests/

QT += widgets

links(robots-kit-base)

INCLUDEPATH += \
	../../../../../../../plugins/robots/common/kitBase/include/ \
	../../../../../../../plugins/robots/utils/include/ \
	../../../../../../../qrgui/ \
	../../../../../../../ \
	../../../../../../../qrtext/include \

HEADERS += \
	mockKitPlugin2Interface.h \

SOURCES += \
	mockKitPlugin2Interface.cpp \
