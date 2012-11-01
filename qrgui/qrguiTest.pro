QT += svg xml

CONFIG += rpath_libdirs help
macx {
	CONFIG -= app_bundle
}

RESOURCES = qrgui.qrc
SOURCES = mainTest.cpp

TRANSLATIONS = qrgui_ru.ts

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$$PWD/../bin/"
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
}

LIBS += -L../bin -lqrrepo -lqrkernel -lqrutils #-lqrmc

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_

INCLUDEPATH += \
	../thirdparty/gmock-1.6.0/include \
	../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L../bin/thirdparty/ -lgmock -lpthread

TARGET = qrgui_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST

include (umllib/umllib.pri)
include (dialogs/dialogs.pri)
include (mainwindow/mainwindow.pri)
include (view/view.pri)
include (generators/generators.pri)
include (thirdparty/thirdparty.pri)
include (pluginManager/pluginManager.pri)
include (models/models.pri)
include (editorPluginInterface/editorPluginInterface.pri)
include (toolPluginInterface/toolPluginInterface.pri)
include (textEditor/textEditor.pri)
include (unitTests/unitTests.pri)
