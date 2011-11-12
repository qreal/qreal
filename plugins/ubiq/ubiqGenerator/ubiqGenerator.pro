TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

LIBS += -L../../../bin -lqrkernel -lqrutils


HEADERS = \
	customizer.h \
	ubiqGeneratorPlugin.h \
	generator.h \

SOURCES = \
	ubiqGeneratorPlugin.cpp \
	generator.cpp \

