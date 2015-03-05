#QT += xml

#CONFIG += c++11

#CONFIG += console
#TARGET = qrmc

#DEPENDPATH += . \
#	templates

#INCLUDEPATH += \
#	$$PWD/.. \

#DESTDIR = ../bin

#!macx {
#	CONFIG += rpath_libdirs
#	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD),-rpath,$(PWD)/../bin"
#}

# Input
#HEADERS += \
#	metaCompiler.h \
#	../qrkernel/definitions.h \
#	../qrkernel/ids.h \
#	../qrrepo/repoApi.h \
#	diagram.h \
#	editor.h \

#SOURCES += \
#	metaCompiler.cpp \
#	diagram.cpp \
#	editor.cpp \
#	main.cpp \

#LIBS += -L../bin -lqrrepo -lqrkernel -lqrutils
<<<<<<< HEAD

#OBJECTS_DIR = .obj
#UI_DIR = .ui
#MOC_DIR = .moc
#RCC_DIR = .moc

#include (classes/classes.pri)
#include (utils/utils.pri)




=======

#OBJECTS_DIR = .obj
#UI_DIR = .ui
#MOC_DIR = .moc
#RCC_DIR = .moc

#include (classes/classes.pri)
#include (utils/utils.pri)
>>>>>>> 1ea90c5095a65415ed9737b91a129f4de4b4160c
include(../global.pri)
include (classes/classes.pri)
include (utils/utils.pri)


HEADERS += \
	metaCompiler.h \
	../qrkernel/definitions.h \
	../qrkernel/ids.h \
	../qrrepo/repoApi.h \
	diagram.h \
	editor.h \

SOURCES += \
	metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \
	main.cpp \

QT += xml

CONFIG += c++11

CONFIG += console
TARGET = qrmc
                 
links(qrutils)
#links(bin)
links(qrrepo)
links(qrkernel)