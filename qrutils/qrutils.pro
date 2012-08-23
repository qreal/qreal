TEMPLATE = lib

QT += xml

DESTDIR = ../bin

TRANSLATIONS += qrutils_ru.ts

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../bin -lqrkernel

DEFINES += QRUTILS_LIBRARY

HEADERS += \
	$$PWD/outFile.h \
	$$PWD/utilsDeclSpec.h \
	$$PWD/xmlUtils.h \
	$$PWD/ui_watchListWindow.h \
	$$PWD/watchListWindow.h \
	$$PWD/metamodelGeneratorSupport.h \
	$$PWD/inFile.h \
    $$PWD/fileSystemUtils.h \

SOURCES += \
	$$PWD/outFile.cpp \
	$$PWD/xmlUtils.cpp \
	$$PWD/watchListWindow.cpp\
	$$PWD/metamodelGeneratorSupport.cpp \
	$$PWD/inFile.cpp \
    $$PWD/fileSystemUtils.cpp \

FORMS += \
	$$PWD/watchListWindow.ui

# Files for parsing of expressions on diagrams
include(expressionsParser/expressionsParser.pri)

# Graphical scene framework used in qrgui and plugins
include(graphicsUtils/graphicsUtils.pri)

# Stuff used in VCS plugins
include(versioningUtils/versioningUtils.pri)

# Stuff connected with graph transformation
include(graphTransformation/graphTransformation.pri)

# Stuff for running long operations in different modes
include(invocationUtils/invocationUtils.pri)
