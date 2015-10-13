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

TARGET = robots-nxt-osek-c-generator

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/nxt/nxtGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-nxt-generator-base)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtOsekCGenerator_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/nxtOsekCGenerator_fr.ts \

HEADERS += \
	$$PWD/nxtOsekCGeneratorPlugin.h \
	$$PWD/nxtOsekCMasterGenerator.h \
	$$PWD/nxtFlashTool.h \

SOURCES += \
	$$PWD/nxtOsekCGeneratorPlugin.cpp \
	$$PWD/nxtOsekCMasterGenerator.cpp \
	$$PWD/nxtFlashTool.cpp \

RESOURCES = \
	$$PWD/nxtOsekCGenerator.qrc \
	$$PWD/templates.qrc \
