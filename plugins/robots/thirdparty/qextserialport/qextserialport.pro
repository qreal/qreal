include(../../../../global.pri)

TEMPLATE = lib

unix:QT = core
else:QT = core gui

include(src/qextserialport.pri)
include(doc/doc.pri)

CONFIG += warn_off

DEFINES += QEXTSERIALPORT_BUILD_SHARED
