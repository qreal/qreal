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

TARGET = qrmc

include(../global.pri)

QT += xml

CONFIG += console

DEPENDPATH += \
	. \
	templates \

HEADERS += \
	$$PWD/metaCompiler.h \
	$$PWD/diagram.h \
	$$PWD/editor.h \
	$$PWD/classes/nonGraphicType.h \
	$$PWD/classes/nodeType.h \
	$$PWD/classes/graphicType.h \
	$$PWD/classes/enumType.h \
	$$PWD/classes/edgeType.h \
	$$PWD/classes/type.h \
	$$PWD/classes/property.h \
	$$PWD/classes/shape.h \
	$$PWD/classes/label.h \
	$$PWD/classes/linePort.h \
	$$PWD/classes/pointPort.h \
	$$PWD/classes/port.h \
	$$PWD/classes/scalableCoordinate.h \
	$$PWD/utils/nameNormalizer.h \
	$$PWD/utils/defs.h \

SOURCES += \
	$$PWD/metaCompiler.cpp \
	$$PWD/diagram.cpp \
	$$PWD/editor.cpp \
	$$PWD/main.cpp \
	$$PWD/classes/nonGraphicType.cpp \
	$$PWD/classes/nodeType.cpp \
	$$PWD/classes/graphicType.cpp \
	$$PWD/classes/enumType.cpp \
	$$PWD/classes/edgeType.cpp \
	$$PWD/classes/type.cpp \
	$$PWD/classes/property.cpp \
	$$PWD/classes/shape.cpp \
	$$PWD/classes/label.cpp \
	$$PWD/classes/linePort.cpp \
	$$PWD/classes/port.cpp \
	$$PWD/classes/pointPort.cpp \
	$$PWD/classes/scalableCoordinate.cpp \
	$$PWD/utils/nameNormalizer.cpp \

TRANSLATIONS += \
	$$PWD/../qrtranslations/ru/qrmc_ru.ts \

OTHER_FILES += \
	$$PWD/qrmcTemplates/edge.template \
	$$PWD/qrmcTemplates/elements.h.template \
	$$PWD/qrmcTemplates/node.template \
	$$PWD/qrmcTemplates/plugin.qrc.template \
	$$PWD/qrmcTemplates/pluginInterface.h.template \
	$$PWD/qrmcTemplates/pluginInterface.cpp.template \
	$$PWD/qrmcTemplates/plugins.pro.template \
	$$PWD/qrmcTemplates/pro.template \
	$$PWD/qrmcTemplates/utils.template \
	$$PWD/plugins/pluginsCommon.pri \
	$$PWD/plugins/pluginsSdk.pri \

links(qrkernel qrutils qrrepo)
includes(qrkernel)

copyToDestdir(qrmcTemplates, now)
