QT += xml

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/hascolSupport_ru.ts

HEADERS += \
	hascolSupportPlugin.h \
	hascolGenerator.h \
	hascolParser.h \

SOURCES += \
	hascolSupportPlugin.cpp \
	hascolGenerator.cpp \
	hascolParser.cpp \
