QT += xml

CONFIG += console
TARGET = qrmc

DEPENDPATH += . \
	templates

DESTDIR = ../bin

!macx {
	CONFIG += rpath_libdirs
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD),-rpath,$(PWD)/../bin"
}

# Input
HEADERS += \
	metaCompiler.h \
	../qrkernel/definitions.h \
	../qrkernel/ids.h \
	../qrrepo/repoApi.h \
	diagram.h \
	editor.h

SOURCES += \
	metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \
	main.cpp

LIBS += -L../bin -lqrrepo -lqrkernel -lqrutils

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

include (classes/classes.pri)
include (utils/utils.pri)
