TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../../../../../bin/plugins/unittests/

QT += widgets

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

LIBS += -L../../../../../../../bin -linterpreterBase

INCLUDEPATH += \
	../../../../../../../plugins/robots/interpreters/interpreterBase/include/ \
	../../../../../../../plugins/robots/utils/include/ \
	../../../../../../../qrgui/ \
	../../../../../../../ \

HEADERS += \
	mockKitPlugin2Interface.h \

SOURCES += \
	mockKitPlugin2Interface.cpp \
