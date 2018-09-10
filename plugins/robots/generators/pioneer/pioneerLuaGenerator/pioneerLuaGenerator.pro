# Copyright 2017 QReal Research Group
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

TARGET = robots-pioneer-lua-generator

include(../../../../../global.pri)


QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes( \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/pioneerKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 qrgui-text-editor qrgui-preferences-dialog qrtext robots-kit-base \
		robots-generator-base robots-pioneer-kit robots-utils)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/pioneerLuaGenerator_ru.ts

HEADERS += \
	$$PWD/pioneerLuaGeneratorCustomizer.h \
	$$PWD/pioneerLuaGeneratorFactory.h \
	$$PWD/pioneerLuaGeneratorPlugin.h \
	$$PWD/pioneerLuaMasterGenerator.h \
	$$PWD/communicator/communicatorInterface.h \
	$$PWD/communicator/communicationManager.h \
	$$PWD/communicator/controllerCommunicator.h \
	$$PWD/communicator/httpCommunicator.h \
	$$PWD/generators/gotoLabelManager.h \
	$$PWD/generators/pioneerStateMachineGenerator.h \
	$$PWD/generators/randomFunctionChecker.h \
	$$PWD/generators/semanticTreeManager.h \
	$$PWD/parts/ledPart.h \
	$$PWD/parts/magnetPart.h \
	$$PWD/parts/tofPart.h \
	$$PWD/parts/randomGeneratorPart.h \
	$$PWD/robotModel/pioneerGeneratorRobotModel.h \
	$$PWD/simpleGenerators/endOfHandlerGenerator.h \
	$$PWD/simpleGenerators/geoLandingGenerator.h \
	$$PWD/simpleGenerators/geoTakeoffGenerator.h \
	$$PWD/simpleGenerators/gotoGenerator.h \
	$$PWD/simpleGenerators/goToPointGenerator.h \
	$$PWD/simpleGenerators/pioneerMagnetGenerator.h \
	$$PWD/simpleGenerators/initialNodeGenerator.h \
	$$PWD/simpleGenerators/labelGenerator.h \
	$$PWD/simpleGenerators/pioneerPrintGenerator.h \
	$$PWD/simpleGenerators/pioneerSystemGenerator.h \
	$$PWD/simpleGenerators/pioneerGetLPSPosition.h \
	$$PWD/simpleGenerators/pioneerLedGenerator.h \
	$$PWD/simpleGenerators/pioneerYawGenerator.h \
	$$PWD/simpleGenerators/randomInitGenerator.h \
	$$PWD/simpleGenerators/pioneerReadRangeSensor.h \
	$$PWD/simpleGenerators/goToGPSPointGenerator.h \
	$$PWD/widgets/pioneerAdditionalPreferences.h \

SOURCES += \
	$$PWD/pioneerLuaGeneratorCustomizer.cpp \
	$$PWD/pioneerLuaGeneratorFactory.cpp \
	$$PWD/pioneerLuaGeneratorPlugin.cpp \
	$$PWD/pioneerLuaMasterGenerator.cpp \
	$$PWD/communicator/communicationManager.cpp \
	$$PWD/communicator/controllerCommunicator.cpp \
	$$PWD/communicator/httpCommunicator.cpp \
	$$PWD/generators/gotoLabelManager.cpp \
	$$PWD/generators/pioneerStateMachineGenerator.cpp \
	$$PWD/generators/randomFunctionChecker.cpp \
	$$PWD/generators/semanticTreeManager.cpp \
	$$PWD/parts/ledPart.cpp \
	$$PWD/parts/magnetPart.cpp \
	$$PWD/parts/randomGeneratorPart.cpp \
	$$PWD/parts/tofPart.cpp \
	$$PWD/robotModel/pioneerGeneratorRobotModel.cpp \
	$$PWD/simpleGenerators/endOfHandlerGenerator.cpp \
	$$PWD/simpleGenerators/geoLandingGenerator.cpp \
	$$PWD/simpleGenerators/geoTakeoffGenerator.cpp \
	$$PWD/simpleGenerators/gotoGenerator.cpp \
	$$PWD/simpleGenerators/goToPointGenerator.cpp \
	$$PWD/simpleGenerators/pioneerMagnetGenerator.cpp \
	$$PWD/simpleGenerators/initialNodeGenerator.cpp \
	$$PWD/simpleGenerators/labelGenerator.cpp \
	$$PWD/simpleGenerators/pioneerGetLPSPosition.cpp \
	$$PWD/simpleGenerators/pioneerPrintGenerator.cpp \
	$$PWD/simpleGenerators/pioneerSystemGenerator.cpp \
	$$PWD/simpleGenerators/pioneerLedGenerator.cpp \
	$$PWD/simpleGenerators/pioneerYawGenerator.cpp \
	$$PWD/simpleGenerators/randomInitGenerator.cpp \
	$$PWD/simpleGenerators/goToGPSPointGenerator.cpp \
	$$PWD/simpleGenerators/pioneerReadRangeSensor.cpp \
	$$PWD/widgets/pioneerAdditionalPreferences.cpp \

FORMS += \
	$$PWD/widgets/pioneerAdditionalPreferences.ui \

RESOURCES += \
	$$PWD/pioneerLuaGenerator.qrc \
	$$PWD/templates.qrc \

OTHER_FILES += \
	$$PWD/pioneerLuaDefaultSettings.ini \
