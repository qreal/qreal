TEMPLATE = app
QT += xml gui
CONFIG += console
CONFIG += c++11

macx {
	CONFIG -= app_bundle
}

MOC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../bin -lqrutils

INCLUDEPATH += \
	$$PWD/..

DESTDIR += ../bin

!macx {
        QMAKE_LFLAGS="-Wl,-O1,-rpath,$$PWD/../bin"
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
	sdftocpp.h \
	stringType.h \
	type.h \
	xmlCompiler.h \
	portType.h

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
	sdftocpp.cpp \
	stringType.cpp \
	type.cpp \
	xmlCompiler.cpp \
	portType.cpp
