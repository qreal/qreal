#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T22:41:49
#
#-------------------------------------------------

QT       += core gui
QT += xml

TARGET = NeuralNetwork
TEMPLATE = app


SOURCES += main.cpp\
		neuralnetworkwindow.cpp \
	classifier.cpp \
	xmlparser.cpp \
    paintmanager.cpp

HEADERS  += neuralnetworkwindow.h \
	neuralnetwork.h \
	classifier.h \
	xmlparser.h \
    adopter.h \
    paintmanager.h

FORMS    += neuralnetworkwindow.ui
