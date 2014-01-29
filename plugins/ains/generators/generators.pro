TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

CONFIG += c++11

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = ains_ru.ts


HEADERS = \
	generators.h \
	databaseSchemeGenerator/databaseSchemeGenerator.h \
        
SOURCES = \
        generators.cpp \
	databaseSchemeGenerator/databaseSchemeGenerator.cpp \

RESOURCES += \
	generators.qrc \
