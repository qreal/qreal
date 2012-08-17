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

#UNIT_TEST = TRUE
!isEmpty(UNIT_TEST) {
	OBJECTS_DIR = .unittestobj
	UI_DIR = .unittestui
	MOC_DIR = .unittestmoc
	RCC_DIR = .unittestmoc

	INCLUDEPATH += \
		../thirdparty/gmock-1.6.0/include \
		../thirdparty/gmock-1.6.0/gtest/include \

	LIBS += -L../bin/thirdparty/ -lgmock -lpthread

	TARGET = qrxc_unittests
	DESTDIR = ../bin/unittests
	DEFINES += UNITTEST
}

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
	property.h \
	scalableCoordinate.h \
	sdftocpp.h \
	stringType.h \
	type.h \
	xmlCompiler.h \
	scalableItem.h

SOURCES += association.cpp \
	diagram.cpp \
	edgeType.cpp \
	editor.cpp \
	enumType.cpp \
	graphicType.cpp \
	label.cpp \
	linePort.cpp \
	main.cpp \
	nameNormalizer.cpp \
	nodeType.cpp \
	nonGraphicType.cpp \
	numericType.cpp \
	pointPort.cpp \
	port.cpp \
	property.cpp \
	scalableCoordinate.cpp \
	sdftocpp.cpp \
	stringType.cpp \
	type.cpp \
	xmlCompiler.cpp \
	scalableItem.cpp

# Unit tests
include (unitTests/unitTests.pri)
