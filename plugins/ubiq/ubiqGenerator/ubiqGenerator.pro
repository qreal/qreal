TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

LIBS += -L../../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
MOC_DIR = .moc

HEADERS = \
        customizer.h \
        ubiqGeneratorPlugin.h \
        generator.h \
        messageGenerator.h \
        devRecordGenerator.h \
    abstractGenerator.h

SOURCES = \
        ubiqGeneratorPlugin.cpp \
        generator.cpp \
        messageGenerator.cpp \
        devRecordGenerator.cpp \
    abstractGenerator.cpp




