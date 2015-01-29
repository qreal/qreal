TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-facade

QT += widgets

LIBS += -L$$PWD/../../bin -lqrkernel -lqrgui-models -lqrgui-plugin-manager -lqrgui-controller

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_system_facade_ru.ts

DEFINES += QRGUI_SYSTEM_FACADE_LIBRARY

HEADERS += \
	$$PWD/systemFacade.h \

SOURCES += \
	$$PWD/systemFacade.cpp \
