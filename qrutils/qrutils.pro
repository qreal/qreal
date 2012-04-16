TEMPLATE = lib

QT += xml

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../bin -lqrkernel

DEFINES += QRUTILS_LIBRARY

HEADERS += \
	outFile.h \
	utilsDeclSpec.h \
	xmlUtils.h \
	ui_watchListWindow.h \
	watchListWindow.h \

SOURCES += \
	outFile.cpp \
	xmlUtils.cpp \
	watchListWindow.cpp\

FORMS += \
	watchListWindow.ui

# Files for parsing of expressions on diagrams
include(expressionsParser/expressionsParser.pri)

# Graphical scene framework used in qrgui and plugins
include(graphicsUtils/graphicsUtils.pri)

# Generator utils used in plugins
include(generatorsUtils/generatorsUtils.pri)
