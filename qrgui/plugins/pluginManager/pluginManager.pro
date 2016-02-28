# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET = qrgui-plugin-manager

include(../../../global.pri)

TEMPLATE = lib

QT += xml svg

links(qrkernel qslog qrutils qrrepo qrgui-tool-plugin-interface qrgui-meta-meta-model)

includes(qrgui qrgraph qrgui/plugins/metaMetaModel)

DEFINES += QRGUI_PLUGINS_MANAGER_LIBRARY

TRANSLATIONS = \
	$$PWD/../../../qrtranslations/ru/qrgui_pluginsManager_ru.ts \
	$$PWD/../../../qrtranslations/fr/qrgui_pluginsManager_fr.ts \

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

RESOURCES += \
	$$PWD/pluginManager.qrc \
