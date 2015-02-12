include(../../../global.pri)

TEMPLATE = lib

CONFIG += plugin

copyToDestdir(templates)

DESTDIR = $$DESTDIR/plugins/tools

links(qrkernel qrutils)

HEADERS = \
	customizer.h \
	ubiqGeneratorPlugin.h \
	generator.h \
	messageGenerator.h \
	customClassGenerator.h \
	dispatcherGenerator.h \

SOURCES = \
	ubiqGeneratorPlugin.cpp \
	generator.cpp \
	messageGenerator.cpp \
	customClassGenerator.cpp \
	dispatcherGenerator.cpp \

QT += widgets