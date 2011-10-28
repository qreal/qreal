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

SOURCES += \
    outFile.cpp \
    xmlUtils.cpp \

# Files for parsing of expressions on diagrams
include(expressionsParser/expressionsParser.pri)

# Graphical scene framework used in qrgui and plugins
include(graphicsUtils/graphicsUtils.pri)
