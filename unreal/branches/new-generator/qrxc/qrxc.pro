TEMPLATE = app
TARGET = qrxc
DESTDIR = ./debug
QT += xml gui
CONFIG += console
DEFINES += QT_LARGEFILE_SUPPORT QT_DLL QT_XML_LIB
INCLUDEPATH += ./.moc
DEPENDPATH += .
MOC_DIR += ./.moc
OBJECTS_DIR += debug

#Include file(s)
include(qrxc.pri)
