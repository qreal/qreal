TEMPLATE = app
QT += xml gui
CONFIG += console

macx {
	CONFIG -= app_bundle
}

MOC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../bin -lqrutils

DESTDIR += ../bin

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$$PWD/../bin"
}

INCLUDEPATH += \
	../thirdparty/gmock-1.6.0/include \
	../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L../bin/thirdparty/ -lgmock -lpthread

TARGET = qrxc_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST

HEADERS += association.h \
	diagram.h \
	edgeType.h \
	editor.h \
	enumType.h \
	graphicType.h \
	label.h \
	linePort.h \
	nameNormalizer.h \
	nodeType.h \
	nonGraphicType.h \
	numericType.h \
	pointPort.h \
	port.h \
	portType.h \
	property.h \
	sdftocpp.h \
	stringType.h \
	type.h \
	xmlCompiler.h \

SOURCES += association.cpp \
	diagram.cpp \
	edgeType.cpp \
	editor.cpp \
	enumType.cpp \
	graphicType.cpp \
	label.cpp \
	linePort.cpp \
	mainTest.cpp \
	nameNormalizer.cpp \
	nodeType.cpp \
	nonGraphicType.cpp \
	numericType.cpp \
	pointPort.cpp \
	port.cpp \
	portType.cpp \
	property.cpp \
	sdftocpp.cpp \
	stringType.cpp \
	type.cpp \
	xmlCompiler.cpp \

# Unit tests
include (unitTests/unitTests.pri)
