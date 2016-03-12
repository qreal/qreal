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

TARGET = qrgui-tool-plugin-interface

include(../../../global.pri)

TEMPLATE = lib

links(qrkernel)
includes(qrgui)

DEFINES += QRGUI_TOOL_PLUGIN_INTERFACE_LIBRARY

TRANSLATIONS = \
	$$PWD/../../../qrtranslations/ru/qrgui_toolPluginInterface_ru.ts \
	$$PWD/../../../qrtranslations/fr/qrgui_toolPluginInterface_fr.ts \

HEADERS += \
	$$PWD/toolPluginInterfaceDeclSpec.h \
	$$PWD/toolPluginInterface.h \
	$$PWD/customizer.h \
	$$PWD/pluginConfigurator.h \
	$$PWD/actionInfo.h \
	$$PWD/tabInfo.h \
	$$PWD/usedInterfaces/progressReporterInterface.h \
	$$PWD/usedInterfaces/errorReporterInterface.h \
	$$PWD/usedInterfaces/details/modelsAssistInterface.h \
	$$PWD/usedInterfaces/graphicalModelAssistInterface.h \
	$$PWD/usedInterfaces/logicalModelAssistInterface.h \
	$$PWD/usedInterfaces/projectManagementInterface.h \
	$$PWD/usedInterfaces/mainWindowInterpretersInterface.h \
	$$PWD/usedInterfaces/mainWindowDockInterface.h \
	$$PWD/projectConverter.h \
	$$PWD/hotKeyActionInfo.h \
	$$PWD/systemEvents.h \
	$$PWD/pluginInterface.h \
