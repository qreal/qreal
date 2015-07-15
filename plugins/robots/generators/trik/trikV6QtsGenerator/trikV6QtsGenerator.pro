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

TARGET = robots-trik-v6-qts-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/trik/trikV6GeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-v6-generator-base \
		robots-kit-base robots-trik-kit robots-utils \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikV6QtsGenerator_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikQtsGenerator_fr.ts \

HEADERS += \
	$$PWD/trikV6QtsGeneratorPlugin.h \
	$$PWD/trikV6QtsMasterGenerator.h \
	$$PWD/trikV6QtsControlFlowValidator.h \
	$$PWD/threadsValidator.h \
	$$PWD/emptyShell.h \

SOURCES += \
	$$PWD/trikV6QtsGeneratorPlugin.cpp \
	$$PWD/trikV6QtsMasterGenerator.cpp \
	$$PWD/trikV6QtsControlFlowValidator.cpp \
	$$PWD/threadsValidator.cpp \
	$$PWD/emptyShell.cpp \

RESOURCES = \
	$$PWD/trikV6QtsGenerator.qrc \
	$$PWD/templates.qrc \
