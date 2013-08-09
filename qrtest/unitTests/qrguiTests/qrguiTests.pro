TEMPLATE = app
CONFIG += console

QT += svg xml printsupport widgets help

DESTDIR = ../../../bin
TARGET = qrgui_unittests

macx {
	CONFIG -= app_bundle
}

SOURCES = mainTest.cpp

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,."
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
}

INCLUDEPATH += \
	../../../thirdparty/gmock-1.6.0/include \
	../../../thirdparty/gmock-1.6.0/gtest/include \
	../../../qrgui/.ui \
	../../../qrgui/ \

LIBS += -L../../../bin/thirdparty/ -lgmock -lpthread

include(../../../qrgui/qrgui.pri)

include(modelsTests/modelsTests.pri)
