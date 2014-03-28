TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

QT += widgets

LIBS += -L../../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
MOC_DIR = .moc


HEADERS = \
	generatorsGeneratorPlugin.h \

SOURCES = \
	generatorsGeneratorPlugin.cpp \
