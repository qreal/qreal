
TEMPLATE = app
TARGET = VisualStudioProjectsFix
DESTDIR = ./release
CONFIG += debug console
DEFINES += QT_LARGEFILE_SUPPORT
INCLUDEPATH += ./.moc/Debug
DEPENDPATH += .
MOC_DIR += ./.moc/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(VisualStudioProjectsFix.pri)
