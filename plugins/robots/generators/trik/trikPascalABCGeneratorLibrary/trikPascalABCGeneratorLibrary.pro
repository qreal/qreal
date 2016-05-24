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

TARGET = robots-trik-pascal-abc-generator-library

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base \
		robots-kit-base robots-trik-kit robots-utils qrgui-preferences-dialog \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikPascalABCGeneratorLibrary_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikPascalABCGeneratorLibrary_fr.ts \

HEADERS += \
	$$PWD/include/trikPascalABCGeneratorLibrary/trikPascalABCGeneratorPluginBase.h \
	$$PWD/src/trikPascalABCAdditionalPreferences.h \
	$$PWD/src/trikPascalABCControlFlowValidator.h \
	$$PWD/src/trikPascalABCMasterGenerator.h \

SOURCES += \
	$$PWD/src/trikPascalABCAdditionalPreferences.cpp \
	$$PWD/src/trikPascalABCControlFlowValidator.cpp \
	$$PWD/src/trikPascalABCGeneratorPluginBase.cpp \
	$$PWD/src/trikPascalABCMasterGenerator.cpp \

RESOURCES += \
	$$PWD/trikPascalABCGeneratorLibrary.qrc \
	$$PWD/templates.qrc \

FORMS += \
	$$PWD/src/trikPascalABCAdditionalPreferences.ui \
