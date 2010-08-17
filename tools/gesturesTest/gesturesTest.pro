#-------------------------------------------------
#
# Project created by QtCreator 2010-08-11T22:48:36
#
#-------------------------------------------------

QT       += core gui
QT += xml

TARGET = gesturesTest
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	qtAlgorithm/mousegesturerecognizer.cpp \
	rectangleAlgorithm/levenshteindistance.cpp \
	rectangleAlgorithm/keyBuilder.cpp \
	xmlparser.cpp \
    paintmanager.cpp \
    rectangleAlgorithm/pathcorrector.cpp \
    gesturesmanager.cpp

HEADERS  += mainwindow.h \
	qtAlgorithm/mousegesturerecognizer.h \
	rectangleAlgorithm/levenshteindistance.h \
	rectangleAlgorithm/keyBuilder.h \
	xmlparser.h \
    paintmanager.h \
    rectangleAlgorithm/pathcorrector.h \
    gesturesmanager.h \
    adopter.h

FORMS    += mainwindow.ui
