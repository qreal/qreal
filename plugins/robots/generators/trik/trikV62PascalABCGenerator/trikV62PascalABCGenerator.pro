# Copyright 2016 Ivan Limar
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

TARGET = robots-trik-v62-pascal-abc-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes( \
		plugins/robots/generators/trik/trikPascalABCGeneratorLibrary \
		plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
		thirdparty/qscintilla/Qt4Qt5 \
)

links(robots-trik-pascal-abc-generator-library robots-trik-generator-base robots-generator-base robots-trik-kit \
				robots-kit-base \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikV62PascalABCGenerator_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikV62PascalABCGenerator_fr.ts \


HEADERS += \
	$$PWD/trikV62PascalABCGeneratorPlugin.h

SOURCES += \
	$$PWD/trikV62PascalABCGeneratorPlugin.cpp
