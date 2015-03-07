#-------------------------------------------------
#
# Project created by QtCreator 2015-03-02T15:49:31
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = generationRulesTool_unittests
DESTDIR = $$PWD/bin/$$CONFIGURATION

include(../../common.pri)
include(../../../../plugins/generationRulesTool/generationRulesTool.pri)

TEMPLATE = app

HEADERS += \
	lexerTest.h \
	parserTest.h

SOURCES += \
	lexerTest.cpp \
	parserTest.cpp

INCLUDEPATH += \
	$$PWD/../../../../ \
	$$PWD/../../../thirdparty/gmock-1.6.0/gtest/include \
