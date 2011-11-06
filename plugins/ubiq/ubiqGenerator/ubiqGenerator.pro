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
	customClassGenerator.h \
	abstractGenerator.h \
	nameNormalizer.h \
	dispatcherGenerator.h

SOURCES = \
	ubiqGeneratorPlugin.cpp \
	generator.cpp \
	messageGenerator.cpp \
	customClassGenerator.cpp \
	abstractGenerator.cpp \
	nameNormalizer.cpp \
	dispatcherGenerator.cpp





