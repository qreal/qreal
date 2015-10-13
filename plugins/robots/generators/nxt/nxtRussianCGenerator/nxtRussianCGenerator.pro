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

TARGET = robots-nxt-russian-c-generator

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

links(qrkernel qrutils qrrepo qscintilla2 robots-generator-base robots-nxt-generator-base)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtRussianCGenerator_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/nxtRussianCGenerator_fr.ts \

HEADERS += \
	$$PWD/nxtRussianCGeneratorPlugin.h \
	$$PWD/nxtRussianCMasterGenerator.h \

SOURCES += \
	$$PWD/nxtRussianCGeneratorPlugin.cpp \
	$$PWD/nxtRussianCMasterGenerator.cpp \

RESOURCES = \
	$$PWD/nxtRussianCGenerator.qrc \
	$$PWD/templates.qrc \
