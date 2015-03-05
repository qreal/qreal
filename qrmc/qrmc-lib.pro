#TEMPLATE = lib
#QT += xml
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
