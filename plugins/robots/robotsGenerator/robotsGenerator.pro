QT += widgets xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGenerator_ru.ts

RESOURCES = robotsGenerator.qrc

HEADERS += \
	robotsGeneratorPlugin.h \

SOURCES += \
	robotsGeneratorPlugin.cpp \

include(nxtOSEK/nxtOsek.pri)
