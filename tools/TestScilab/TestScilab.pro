# -------------------------------------------------
# Project created by QtCreator 2010-07-03T12:36:58
# -------------------------------------------------
QT -= gui
TARGET = TestScilab
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    MatrixWork.cpp
INCLUDEPATH += D:\QT\scilab-5.2.2\modules\api_scilab\includes
INCLUDEPATH += D:\QT\scilab-5.2.2\modules\core\includes
INCLUDEPATH += D:\QT\scilab-5.2.2\modules\call_scilab\includes
LIBS += D:\QT\scilab-5.2.2\bin\api_scilab.dll
LIBS += D:\QT\scilab-5.2.2\bin\call_scilab.dll
LIBS += D:\QT\scilab-5.2.2\bin\LibScilab.dll
HEADERS += MatrixWork.h
