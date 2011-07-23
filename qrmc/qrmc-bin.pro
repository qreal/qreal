QT += xml

TARGET = qrmc

DEPENDPATH += . \
	templates

DESTDIR = ../bin

!macx {
	CONFIG += rpath_libdirs
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD),-rpath,$(PWD)/../qrgui"
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

LIBS += -L../bin -lqrrepo -lqrkernel

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

include (classes/classes.pri)
include (utils/utils.pri)
