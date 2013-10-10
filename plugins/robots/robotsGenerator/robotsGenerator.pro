QT += widgets

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

DEFINES += ROBOTS_GENERATOR_LIBRARY

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGenerator_ru.ts

RESOURCES = robotsGenerator.qrc

HEADERS += \
	robotsGeneratorPlugin.h \
	robotsGeneratorDeclSpec.h \

SOURCES += \
	robotsGeneratorPlugin.cpp \

include(base/base.pri)

include(nxtOSEK/nxtOsek.pri)
