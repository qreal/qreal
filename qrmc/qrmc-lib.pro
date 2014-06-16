TEMPLATE = lib
QT += xml

CONFIG += c++11

TARGET = qrmc

DEPENDPATH += . \
	templates

INCLUDEPATH += \
	$$PWD/.. \

LIBS += -L../bin -lqrkernel -lqrrepo

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DESTDIR = ../bin

# Input
HEADERS += \
	$$PWD/metaCompiler.h \
	$$PWD/../qrkernel/definitions.h \
	$$PWD/../qrkernel/ids.h \
	$$PWD/../qrrepo/repoApi.h \
	$$PWD/diagram.h \
	$$PWD/editor.h \              

SOURCES += \
	$$PWD/metaCompiler.cpp \
	$$PWD/diagram.cpp \
	$$PWD/editor.cpp \

include (classes/classes.pri)
include (utils/utils.pri)
