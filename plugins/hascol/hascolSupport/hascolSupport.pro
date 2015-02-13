include(../../../global.pri)

QT += xml widgets

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools

links(qrkernel qrutils)

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/hascolSupport_ru.ts

HEADERS += \
	hascolSupportPlugin.h \
	hascolGenerator.h \
	hascolParser.h \

SOURCES += \
	hascolSupportPlugin.cpp \
	hascolGenerator.cpp \
	hascolParser.cpp \
