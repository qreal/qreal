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

TARGET = robots-trik-qts-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base \
		robots-trik-kit robots-utils \
)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikQtsGenerator_ru.ts

HEADERS += \
	$$PWD/trikQtsGeneratorPlugin.h \
	$$PWD/trikQtsMasterGenerator.h \
	$$PWD/trikQtsControlFlowValidator.h \
	$$PWD/threadsValidator.h \
	$$PWD/emptyShell.h \

SOURCES += \
	$$PWD/trikQtsGeneratorPlugin.cpp \
	$$PWD/trikQtsMasterGenerator.cpp \
	$$PWD/trikQtsControlFlowValidator.cpp \
	$$PWD/threadsValidator.cpp \
	$$PWD/emptyShell.cpp \

RESOURCES = \
	$$PWD/trikQtsGenerator.qrc \
	$$PWD/templates.qrc \
