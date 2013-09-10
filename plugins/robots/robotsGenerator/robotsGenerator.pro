QT += widgets xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

INCLUDEPATH += \
	../../../ \
	../../../qrgui \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGenerator_ru.ts

RESOURCES =

HEADERS += \
	robotsGeneratorPlugin.h \

SOURCES += \
	robotsGeneratorPlugin.cpp \

include(base/base.pri)

include(nxtOSEK/nxtOsek.pri)
