QT += xml widgets

TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
        $$PWD/../../.. \
        $$PWD/../../../qrgui/ \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrmc -lqrrepo


HEADERS += \
        generator.h \

SOURCES += \
        generator.cpp \

