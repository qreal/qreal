# Copyright 2016 CyberTech Labs Ltd.
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

TARGET = robots-trik-python-generator-library

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
		thirdparty/qscintilla/Qt4Qt5 \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base \
		robots-kit-base robots-trik-kit robots-utils \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikPythonGeneratorLibrary_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikPythonGeneratorLibrary_fr.ts \

HEADERS += \
	$$PWD/include/trikPythonGeneratorLibrary/trikPythonGeneratorPluginBase.h \
	$$PWD/src/trikPythonMasterGenerator.h \
	$$PWD/src/trikPythonControlFlowValidator.h \
	$$PWD/src/threadsValidator.h \
	$$PWD/src/emptyShell.h \

SOURCES += \
	$$PWD/src/trikPythonGeneratorPluginBase.cpp \
	$$PWD/src/trikPythonMasterGenerator.cpp \
	$$PWD/src/trikPythonControlFlowValidator.cpp \
	$$PWD/src/threadsValidator.cpp \
	$$PWD/src/emptyShell.cpp \

RESOURCES = \
	$$PWD/trikPythonGeneratorLibrary.qrc \
	$$PWD/templates.qrc \
