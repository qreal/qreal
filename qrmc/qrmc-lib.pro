TARGET = qrmc

<<<<<<< HEAD
TEMPLATE = lib
=======
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
>>>>>>> 2a644e69703bdf2b2ea8122ede877834aceddde1

include(qrmc.pri)
