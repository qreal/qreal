include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools/

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets

HEADERS = \
	exterminatusPlugin.h \

SOURCES = \
	exterminatusPlugin.cpp \
