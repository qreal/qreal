#-------------------------------------------------
#
# Project created by QtCreator 2013-02-25T23:07:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nxtDisplay
TEMPLATE = app


SOURCES += main.cpp \
    nxtDisplay.cpp \
    mainWindow.cpp \
    dotMatrixDisplay.cpp

HEADERS  += \
    nxtDisplay.h \
    dotMatrixDisplay.h \
    mainWindow.h

FORMS    += \
    nxtDisplay.ui \
    mainWindow.ui

RESOURCES += \
    images.qrc
