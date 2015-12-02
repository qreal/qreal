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

TARGET = robots-ev3-rbf-generator

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/ev3/ev3GeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/ev3Kit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 qrgui-text-editor qrtext \
		robots-generator-base robots-ev3-generator-base robots-ev3-kit robots-utils)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3RbfGenerator_ru.ts

HEADERS += \
	ev3RbfGeneratorPlugin.h \
	ev3RbfMasterGenerator.h \
	ev3RbfGeneratorCustomizer.h \
	ev3RbfGeneratorFactory.h \
	lua/ev3LuaProcessor.h \
	lua/ev3LuaPrinter.h \
	lua/ev3ReservedFunctionsConverter.h \
	simpleGenerators/prependedCodeGenerator.h \

SOURCES += \
	ev3RbfGeneratorPlugin.cpp \
	ev3RbfMasterGenerator.cpp \
	ev3RbfGeneratorCustomizer.cpp \
	ev3RbfGeneratorFactory.cpp \
	lua/ev3LuaProcessor.cpp \
	lua/ev3LuaPrinter.cpp \
	lua/ev3ReservedFunctionsConverter.cpp \
	simpleGenerators/prependedCodeGenerator.cpp \

RESOURCES += \
	ev3RbfGenerator.qrc \
	templates.qrc \
	thirdparty.qrc \
