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
	common/levenshteindistance.cpp \
	rectangleAlgorithm/keyBuilder.cpp \
	xmlparser.cpp \
	paintmanager.cpp \
	rectangleAlgorithm/pathcorrector.cpp \
	gesturesmanager.cpp \
	chaosStarAlgorithm/key8manager.cpp \

HEADERS  += mainwindow.h \
	qtAlgorithm/mousegesturerecognizer.h \
	common/levenshteindistance.h \
	rectangleAlgorithm/keyBuilder.h \
	xmlparser.h \
	paintmanager.h \
	rectangleAlgorithm/pathcorrector.h \
	gesturesmanager.h \
	adopter.h \
	chaosStarAlgorithm/key8manager.h \

FORMS += mainwindow.ui
