TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/tools/

MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj

QT += widgets

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrgui-versioning-plugins-manager

HEADERS += \
	diffPlugin.h \

SOURCES += \
	diffPlugin.cpp \
