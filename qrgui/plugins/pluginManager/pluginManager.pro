TARGET = qrgui-plugin-manager

include(../../../global.pri)

TEMPLATE = lib

QT += xml svg

links(qrkernel qslog qrutils qrrepo qrgui-tool-plugin-interface)

includes(qrgui)

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

RESOURCES += \
	$$PWD/pluginManager.qrc \
