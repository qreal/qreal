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

TARGET = robots-trik-f-sharp-generator-library

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib

copyToDestdir($$PWD/lib/Trik.Core.dll)

DEFINES += ROBOTS_TRIK_FSHARP_GENERATOR_LIBRARY

includes( \
		plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base robots-utils \
		qrgui-preferences-dialog robots-kit-base \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikFSharpGeneratorLibrary_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikFSharpGeneratorLibrary_fr.ts \

HEADERS += \
	$$PWD/include/trikFSharpGeneratorLibrary/trikFSharpGeneratorPluginBase.h \
	$$PWD/include/trikFSharpGeneratorLibrary/declSpec.h \
	$$PWD/src/trikFSharpMasterGenerator.h \
	$$PWD/src/trikFSharpAdditionalPreferences.h \
	$$PWD/src/trikFSharpControlFlowValidator.h \

SOURCES += \
	$$PWD/src/trikFSharpGeneratorPluginBase.cpp \
	$$PWD/src/trikFSharpMasterGenerator.cpp \
	$$PWD/src/trikFSharpAdditionalPreferences.cpp \
	$$PWD/src/trikFSharpControlFlowValidator.cpp \

FORMS += \
	$$PWD/src/trikFSharpAdditionalPreferences.ui \

RESOURCES = \
	$$PWD/trikFSharpGeneratorLibrary.qrc \
	$$PWD/templates.qrc \
