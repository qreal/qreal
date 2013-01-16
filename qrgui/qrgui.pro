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

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$$PWD/../bin/"
}

win32 {
	QMAKE_POST_LINK = "xcopy DejaVuSansCondensed.ttf ..\\bin /q /y"
}
else {
	QMAKE_POST_LINK = "cp DejaVuSansCondensed.ttf ../bin/"
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

# Graphical elements
include (umllib/umllib.pri)

# Dialogs
include (dialogs/dialogs.pri)

# Main window
include (mainwindow/mainwindow.pri)

# View
include (view/view.pri)

# Built-in generators
include (generators/generators.pri)

# The code from the Internet
include (thirdparty/thirdparty.pri)

# Plugin managment
include (pluginManager/pluginManager.pri)

# Graphical and logical models
include (models/models.pri)

# Interfaces for plugins, used by qrxc and qrmc
include (editorPluginInterface/editorPluginInterface.pri)

# Interfaces for tool plugins, used in handcoded tools
include (toolPluginInterface/toolPluginInterface.pri)

# Text Editor
include (textEditor/textEditor.pri)
