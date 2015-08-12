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

TARGET = robots-nxt-generator-base

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib

includes(plugins/robots/generators/nxt/nxtGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/nxtKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo robots-generator-base robots-nxt-kit robots-kit-base)

DEFINES += ROBOTS_NXT_GENERATOR_BASE_LIBRARY

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtGeneratorBase_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/nxtGeneratorBase_fr.ts \

HEADERS += \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorPluginBase.h \
	$$PWD/include/nxtGeneratorBase/nxtMasterGeneratorBase.h \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorFactory.h \
	$$PWD/include/nxtGeneratorBase/parts/images.h \
	$$PWD/src/nxtGeneratorCustomizer.h \
	$$PWD/src/robotModel/nxtGeneratorRobotModel.h \
	$$PWD/src/simpleGenerators/clearScreenBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawPixelBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawRectBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawLineBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawCircleBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForColorBlockGenerator.h \
	$$PWD/src/converters/colorConverter.h \

SOURCES += \
	$$PWD/src/nxtGeneratorPluginBase.cpp \
	$$PWD/src/nxtMasterGeneratorBase.cpp \
	$$PWD/src/nxtGeneratorCustomizer.cpp \
	$$PWD/src/nxtGeneratorFactory.cpp \
	$$PWD/src/robotModel/nxtGeneratorRobotModel.cpp \
	$$PWD/src/parts/images.cpp \
	$$PWD/src/simpleGenerators/clearScreenBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawPixelBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawRectBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawLineBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawCircleBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForColorBlockGenerator.cpp \
	$$PWD/src/converters/colorConverter.cpp \
