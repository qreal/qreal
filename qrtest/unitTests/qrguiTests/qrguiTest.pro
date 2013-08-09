QT += svg xml printsupport widgets help

CONFIG += rpath_libdirs
macx {
	CONFIG -= app_bundle
}

SOURCES = mainTest.cpp

TRANSLATIONS = qrgui_ru.ts

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

LIBS += -L../../../bin/thirdparty/ -lgmock -lpthread

TARGET = qrgui_unittests
DESTDIR = ../../../bin/

include(../../../qrgui/qrgui.pri)

include(modelsTests/modelsTests.pri)
