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
HEADERS += \
        expressionsParser/expressionsParser.h \
        expressionsParser/number.h \

SOURCES += \
        expressionsParser/expressionsParser.cpp \
        expressionsParser/number.cpp \

## Бред, придуманный в неадекватный день
#include (graphicsUtils/graphicsUtils.pri)
HEADERS += \
        graphicsUtils/abstractItem.h \
        graphicsUtils/lineImpl.h \
        graphicsUtils/abstractScene.h \
        graphicsUtils/abstractItemView.h \
        graphicsUtils/rectangleImpl.h \
        graphicsUtils/pointImpl.h \
        graphicsUtils/rotateInterface.h \
        graphicsUtils/colorlisteditor.h \
        graphicsUtils/stylusImpl.h \

SOURCES += \
        graphicsUtils/abstractItem.cpp \
        graphicsUtils/lineImpl.cpp \
        graphicsUtils/abstractScene.cpp \
        graphicsUtils/abstractItemView.cpp \
        graphicsUtils/rectangleImpl.cpp \
        graphicsUtils/pointImpl.cpp \
        graphicsUtils/colorlisteditor.cpp \
        graphicsUtils/stylusImpl.cpp \
        graphicsUtils/rotateInterface.cpp \
