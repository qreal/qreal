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

TARGET = robots-trik-qts-generator-library

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
		robots-kit-base robots-trik-kit robots-utils \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikQtsGeneratorLibrary_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikQtsGeneratorLibrary_fr.ts \

HEADERS += \
	$$PWD/include/trikQtsGeneratorLibrary/trikQtsGeneratorPluginBase.h \
	$$PWD/src/trikQtsMasterGenerator.h \
	$$PWD/src/trikQtsControlFlowValidator.h \
	$$PWD/src/threadsValidator.h \
	$$PWD/src/emptyShell.h \

SOURCES += \
	$$PWD/src/trikQtsGeneratorPluginBase.cpp \
	$$PWD/src/trikQtsMasterGenerator.cpp \
	$$PWD/src/trikQtsControlFlowValidator.cpp \
	$$PWD/src/threadsValidator.cpp \
	$$PWD/src/emptyShell.cpp \

RESOURCES = \
	$$PWD/trikQtsGeneratorLibrary.qrc \
	$$PWD/templates.qrc \
