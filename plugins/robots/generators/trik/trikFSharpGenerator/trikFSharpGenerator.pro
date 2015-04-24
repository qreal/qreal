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

TARGET = robots-trik-f-sharp-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

copyToDestdir($$PWD/lib/Trik.Core.dll)

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base robots-utils \
		qrgui-preferences-dialog robots-kit-base \
)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikFSharpGenerator_ru.ts

HEADERS += \
	$$PWD/trikFSharpGeneratorPlugin.h \
	$$PWD/trikFSharpMasterGenerator.h \
	$$PWD/trikFSharpAdditionalPreferences.h \
	$$PWD/trikFSharpControlFlowValidator.h \

SOURCES += \
	$$PWD/trikFSharpGeneratorPlugin.cpp \
	$$PWD/trikFSharpMasterGenerator.cpp \
	$$PWD/trikFSharpAdditionalPreferences.cpp \
	$$PWD/trikFSharpControlFlowValidator.cpp \

FORMS += \
	$$PWD/trikFSharpAdditionalPreferences.ui \

RESOURCES = \
	$$PWD/trikFSharpGenerator.qrc \
	$$PWD/templates.qrc \
