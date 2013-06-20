TEMPLATE = lib

QT += xml widgets

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../bin -lqrkernel

DEFINES += QRUTILS_LIBRARY

RESOURCES = qrutils.qrc

TRANSLATIONS = qrutils_ru.ts

HEADERS += \
	$$PWD/outFile.h \
	$$PWD/utilsDeclSpec.h \
	$$PWD/xmlUtils.h \
	$$PWD/watchListWindow.h \
	$$PWD/metamodelGeneratorSupport.h \
	$$PWD/inFile.h \
	$$PWD/scalableItem.h \
	$$PWD/scalableCoordinate.h \

SOURCES += \
	$$PWD/outFile.cpp \
	$$PWD/xmlUtils.cpp \
	$$PWD/watchListWindow.cpp\
	$$PWD/metamodelGeneratorSupport.cpp \
	$$PWD/inFile.cpp \
	$$PWD/scalableItem.cpp \
	$$PWD/scalableCoordinate.cpp \

FORMS += \
	$$PWD/watchListWindow.ui

# Files for parsing of expressions on diagrams
include(expressionsParser/expressionsParser.pri)

# Graphical scene framework used in qrgui and plugins
include(graphicsUtils/graphicsUtils.pri)

# Stuff connected with graphs and trees
include(graphUtils/graphUtils.pri)

# Stuff for running long operations in different modes
include(invocationUtils/invocationUtils.pri)

# Math Utils
include(mathUtils/mathUtils.pri)

# Generator utils used in plugins
include(generatorsUtils/generatorsUtils.pri)
