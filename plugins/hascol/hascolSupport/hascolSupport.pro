QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = hascolSupport_ru.ts

RESOURCES = hascolSupport.qrc

HEADERS += \
	hascolSupportPlugin.h \
	hascolGenerator.h \
	hascolParser.h \

SOURCES += \
	hascolSupportPlugin.cpp \
	hascolGenerator.cpp \
	hascolParser.cpp \
