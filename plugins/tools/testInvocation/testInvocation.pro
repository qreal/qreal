include(../../../../global.pri)

TEMPLATE = lib

CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools

HEADERS += \
	testInvocationPlugin.h \

SOURCES += \
	testInvocationPlugin.cpp \
