TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/tools/
MOC_DIR = .moc
RCC_DIR = .moc

QT += widgets

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L$$PWD/../../../bin -lqrkernel -lqrutils -lqrmc

HEADERS += \
	databasesPhysicalModelPlugin.h \

SOURCES += \
	databasesPhysicalModelPlugin.cpp \
