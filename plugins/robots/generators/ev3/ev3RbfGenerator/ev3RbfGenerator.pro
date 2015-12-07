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
	$$PWD/ev3RbfGeneratorPlugin.h \
	$$PWD/ev3RbfMasterGenerator.h \
	$$PWD/ev3RbfGeneratorCustomizer.h \
	$$PWD/ev3RbfGeneratorFactory.h \
	$$PWD/lua/ev3LuaProcessor.h \
	$$PWD/lua/ev3LuaPrinter.h \
	$$PWD/simpleGenerators/prependedCodeGenerator.h \

SOURCES += \
	$$PWD/ev3RbfGeneratorPlugin.cpp \
	$$PWD/ev3RbfMasterGenerator.cpp \
	$$PWD/ev3RbfGeneratorCustomizer.cpp \
	$$PWD/ev3RbfGeneratorFactory.cpp \
	$$PWD/lua/ev3LuaProcessor.cpp \
	$$PWD/lua/ev3LuaPrinter.cpp \
	$$PWD/simpleGenerators/prependedCodeGenerator.cpp \

RESOURCES += \
	$$PWD/ev3RbfGenerator.qrc \
	$$PWD/templates.qrc \
	$$PWD/thirdparty.qrc \
