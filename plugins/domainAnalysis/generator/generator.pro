include(../../../global.pri)

QT += xml widgets

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools

INCLUDEPATH += \
        $$PWD/../../.. \
        $$PWD/../../../qrgui/ \

links(qrkernel qrutils qrmc qrrepo)


HEADERS += \
        generator.h \

SOURCES += \
        generator.cpp \

