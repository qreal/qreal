#DESTDIR = ../bin

#CONFIG += c++11

#CONFIG += rpath_libdirs
#macx {
#	CONFIG -= app_bundle
#}

#SOURCES = main.cpp

#TRANSLATIONS = $$PWD/../qrtranslations/ru/qrgui_ru.ts

## workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
## when fixed it would become possible to use QMAKE_LFLAGS_RPATH
#!macx {
#	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../bin/
#	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../bin/thirdparty/
#}

#OBJECTS_DIR = .obj
#UI_DIR = .ui
#MOC_DIR = .moc
#RCC_DIR = .moc

#if (equals(QMAKE_CXX, "g++") : !macx) {
#	QMAKE_LFLAGS += -Wl,-E
#}

#include(qrgui.pri)

TEMPLATE = subdirs

SUBDIRS += \
	mainWindow \
	models \
	editor \
	controller \
	dialogs \
	preferencesDialog \
	textEditor \
	mouseGestures \
	hotKeyManager \
	brandManager \
	plugins/pluginManager \
	plugins/editorPluginInterface \
	plugins/toolPluginInterface \
	thirdparty \

mainWindow.depends = \
	models \
	editor \
	controller \
	dialogs \
	preferencesDialog \
	textEditor \
	hotKeyManager \
	brandManager \
	plugins/pluginManager \
	thirdparty \

models.depends = \
	plugins/pluginManager \

editor.depends = \
	models \
	controller \
	mouseGestures \
	brandManager \
	plugins/pluginManager \
	thirdparty \

dialogs.depends = \
	models \
	thirdparty \

textEditor.depends = \
	plugins/toolPluginInterface \

hotKeyManager.depends = \
	preferencesDialog \

brandManager.depends = \
	plugins/pluginManager \

editorPluginInterface.depends =
	plugins/toolPluginInterface
