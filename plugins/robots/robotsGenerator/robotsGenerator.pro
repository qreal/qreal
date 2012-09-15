QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGenerator_ru.ts

RESOURCES = robotsGenerator.qrc

HEADERS += \
	robotsGeneratorPlugin.h \

SOURCES += \
	robotsGeneratorPlugin.cpp \

include(nxtOSEK/nxtOsek.pri)
