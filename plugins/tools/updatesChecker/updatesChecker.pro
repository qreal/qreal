TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/tools/
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets

HEADERS = \
	$$PWD/updatesCheckerPlugin.h \

SOURCES = \
	$$PWD/updatesCheckerPlugin.cpp \
