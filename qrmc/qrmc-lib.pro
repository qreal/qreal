#TEMPLATE = lib
#QT += xml
<<<<<<< HEAD
#
#CONFIG += c++11
#
#TARGET = qrmc
#
#DEPENDPATH += . \
#	templates
#
#INCLUDEPATH += \
#	$$PWD/.. \                          
=======

#CONFIG += c++11

#TARGET = qrmc

#DEPENDPATH += . \
#	templates

<<<<<<< HEAD
#INCLUDEPATH += \
#	$$PWD/.. \                          
=======
INCLUDEPATH += \
	$$PWD/.. \                          
>>>>>>> 2a644e69703bdf2b2ea8122ede877834aceddde1
>>>>>>> 1ea90c5095a65415ed9737b91a129f4de4b4160c

#LIBS += -L../bin -lqrkernel -lqrrepo

#OBJECTS_DIR = .obj
#UI_DIR = .ui
#MOC_DIR = .moc
#RCC_DIR = .moc

#DESTDIR = ../bin

## Input
#HEADERS += \
#	$$PWD/metaCompiler.h \
#	$$PWD/../qrkernel/definitions.h \
#	$$PWD/../qrkernel/ids.h \
#	$$PWD/../qrrepo/repoApi.h \
#	$$PWD/diagram.h \
#	$$PWD/editor.h \              

#SOURCES += \
#	$$PWD/metaCompiler.cpp \
#	$$PWD/diagram.cpp \
#	$$PWD/editor.cpp \

#include (classes/classes.pri)
#include (utils/utils.pri)
<<<<<<< HEAD



=======
>>>>>>> 1ea90c5095a65415ed9737b91a129f4de4b4160c
include (classes/classes.pri)
include (utils/utils.pri)

TARGET = qrmc

include(../global.pri)  #is it correct? o_O

HEADERS += metaCompiler.h \
	  qrkernel/definitions.h \
	  ../qrkernel/ids.h \
	  ../qrrepo/repoApi.h \
	  diagram.h \
	  editor.h \              



SOURCES += metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \

TEMPLATE = lib
QT += xml

CONFIG += c++11



#QT += xml gui
#CONFIG += console #is it correct? o_O

#links(bin)         #is it correct? o_O
links(qrkernel)    #is it correct? o_O
links(qrrepo)      #is it correct? o_O
