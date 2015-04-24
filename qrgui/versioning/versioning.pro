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

TARGET = qrgui-versioning-plugins-manager

include(../../global.pri)

TEMPLATE = lib

QT += widgets

links(qrkernel qrrepo qrutils qrgui-tool-plugin-interface)

includes(qrgui)

DEFINES += QRGUI_VERSIONING_PLUGINS_MANAGER_LIBRARY

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/qrgui_versioningPluginsManager_ru.ts

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

HEADERS += \
	$$PWD/versioningPluginsManagerDeclSpec.h \
	$$PWD/versioningPluginsManager.h \
	$$PWD/versioningPluginInterface.h \
	$$PWD/diffPluginInterface.h \

SOURCES += \
	$$PWD/versioningPluginsManager.cpp \

