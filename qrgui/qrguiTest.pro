DESTDIR = ../bin

QT += svg xml
CONFIG += rpath_libdirs help
macx {
	CONFIG -= app_bundle
}

RESOURCES = qrgui.qrc
SOURCES = main.cpp

TRANSLATIONS = qrgui_ru.ts

# QMAKE_CXXFLAGS_DEBUG += -pg
# QMAKE_LFLAGS_DEBUG += -pg

# workaround для http://bugreports.qt.nokia.com/browse/QTBUG-8110
# как только поправят, можно будет юзать QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD)/../bin/"
}

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
}

LIBS += -L../bin -lqrrepo -lqrkernel -lqrutils #-lqrmc

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_

OBJECTS_DIR = .unittestobj
UI_DIR = .unittestui
MOC_DIR = .unittestmoc
RCC_DIR = .unittestmoc

INCLUDEPATH += ../thirdparty/gmock-1.6.0/include
INCLUDEPATH += ../thirdparty/gmock-1.6.0/gtest/include
LIBS += -L../thirdparty/ -lgmock_main -lpthread

TARGET = qrgui_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST


# Graphical elements
include (umllib/umllib.pri)

# Dialogs
include (dialogs/dialogs.pri)

# Main window
include (mainwindow/mainwindow.pri)

# View
include (view/view.pri)

# "Встроенные" генераторы
include (generators/generators.pri)

# Код, скачанный из интернета.
include (thirdparty/thirdparty.pri)

# Управление плагинами. Plugin managment
include (pluginManager/pluginManager.pri)

# Graphical and logical models
include (models/models.pri)

# Interfaces for plugins, used by qrxc and qrmc.
include (editorPluginInterface/editorPluginInterface.pri)

# Interfaces for tool plugins, used in handcoded tools.
include (toolPluginInterface/toolPluginInterface.pri)

# Text Editor
include (textEditor/textEditor.pri)

# Unit tests for qrgui code
include (unitTests/unitTests.pri)
