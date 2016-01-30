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

include(../../../global.pri)

QT += xml widgets

TEMPLATE = lib
CONFIG += plugin

links(qrkernel qrutils qrmc)
includes(qrgui)

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/constraintsGenerator_ru.ts

HEADERS += \
	constraintsGeneratorPlugin.h \
	generator.h \
	concreteGenerator.h \
	generatorUtils/defs.h \
	generatorParts/generatorForListsOfElements.h \
	generatorParts/generatorForProperties.h \
	generatorParts/generatorForNodeElements.h \
	generatorParts/generatorForElements.h \
	generatorParts/generatorForEdgeElements.h \
	generatorParts/generatorForDiagrams.h \
	generatorParts/generatorForExpressions.h \
	generatorParts/generatorForLinks.h \

SOURCES += \
	constraintsGeneratorPlugin.cpp \
	generator.cpp \
	concreteGenerator.cpp \
	generatorParts/generatorForListsOfElements.cpp \
	generatorParts/generatorForProperties.cpp \
	generatorParts/generatorForNodeElements.cpp \
	generatorParts/generatorForElements.cpp \
	generatorParts/generatorForEdgeElements.cpp \
	generatorParts/generatorForDiagrams.cpp \
	generatorParts/generatorForExpressions.cpp \
	generatorParts/generatorForLinks.cpp \

copyToDestdir(templates/, now)

DESTDIR = $$DESTDIR/plugins/tools
