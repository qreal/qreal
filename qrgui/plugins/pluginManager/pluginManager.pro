TEMPLATE = lib

DESTDIR = $$PWD/../../../bin

TARGET = qrgui-plugin-manager

QT += xml svg

LIBS += -L$$PWD/../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo -lqrgui-tool-plugin-interface

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_PLUGINS_MANAGER_LIBRARY

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/qrgui_pluginsManager_ru.ts

HEADERS += \
	$$PWD/pluginsManagerDeclSpec.h \
	$$PWD/editorManager.h \
	$$PWD/toolPluginManager.h \
	$$PWD/interpreterEditorManager.h \
	$$PWD/editorManagerInterface.h \
	$$PWD/proxyEditorManager.h \
	$$PWD/pattern.h \
	$$PWD/explosion.h \
	$$PWD/sdfRenderer.h \
	$$PWD/interpretedPluginManager.h \
	$$PWD/interpretedPluginsLoader.h \
	$$PWD/details/patternParser.h \
	$$PWD/details/interpreterElementImpl.h \
	$$PWD/details/interpreterPortImpl.h \
	$$PWD/constraintsManager.h \
	$$PWD/../toolPluginInterface/systemEvents.h \

SOURCES += \
	$$PWD/editorManager.cpp \
	$$PWD/toolPluginManager.cpp \
	$$PWD/interpreterEditorManager.cpp \
	$$PWD/proxyEditorManager.cpp \
	$$PWD/pattern.cpp \
	$$PWD/explosion.cpp \
	$$PWD/sdfRenderer.cpp \
	$$PWD/interpretedPluginManager.cpp \
	$$PWD/interpretedPluginsLoader.cpp \
	$$PWD/details/patternParser.cpp \
	$$PWD/details/interpreterElementImpl.cpp \
	$$PWD/details/interpreterPortImpl.cpp \
	$$PWD/constraintsManager.cpp \

RESOURCES += \
	$$PWD/pluginManager.qrc \
