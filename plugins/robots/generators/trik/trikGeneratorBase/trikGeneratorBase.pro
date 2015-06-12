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

TARGET = robots-trik-generator-base

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo robots-generator-base robots-kit-base robots-trik-kit)

DEFINES += ROBOTS_TRIK_GENERATOR_BASE_LIBRARY

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikGeneratorBase_ru.ts

HEADERS += \
	$$PWD/include/trikGeneratorBase/trikGeneratorBaseDeclSpec.h \
	$$PWD/include/trikGeneratorBase/trikGeneratorPluginBase.h \
	$$PWD/include/trikGeneratorBase/trikMasterGeneratorBase.h \
	$$PWD/include/trikGeneratorBase/robotModel/trikGeneratorRobotModel.h \
	$$PWD/src/trikGeneratorCustomizer.h \
	$$PWD/src/trikGeneratorFactory.h \
	$$PWD/src/parts/trikDeviceVariables.h \
	$$PWD/src/converters/backgroundColorConverter.h \
	$$PWD/src/converters/ledColorConverter.h \
	$$PWD/src/simpleGenerators/detectGenerator.h \
	$$PWD/src/simpleGenerators/initCameraGenerator.h \
	$$PWD/src/simpleGenerators/ledGenerator.h \
	$$PWD/src/simpleGenerators/detectorToVariableGenerator.h \
	$$PWD/src/simpleGenerators/playToneGenerator.h \
	$$PWD/src/simpleGenerators/sadSmileGenerator.h \
	$$PWD/src/simpleGenerators/sayGenerator.h \
	$$PWD/src/simpleGenerators/systemGenerator.h \
	$$PWD/src/simpleGenerators/setBackgroundGenerator.h \
	$$PWD/src/simpleGenerators/smileGenerator.h \
	$$PWD/src/simpleGenerators/trikEnginesGenerator.h \
	$$PWD/src/simpleGenerators/waitForInfraredSensorGenerator.h \
	$$PWD/src/simpleGenerators/waitForMotionGenerator.h \
	$$PWD/src/simpleGenerators/drawLineGenerator.h \
	$$PWD/src/simpleGenerators/drawPixelGenerator.h \
	$$PWD/src/simpleGenerators/drawRectGenerator.h \
	$$PWD/src/simpleGenerators/drawEllipseGenerator.h \
	$$PWD/src/simpleGenerators/drawArcGenerator.h \
	$$PWD/src/simpleGenerators/setPainterWidthGenerator.h \
	$$PWD/src/simpleGenerators/setPainterColorGenerator.h \
	$$PWD/src/simpleGenerators/sendMessageGenerator.h \
	$$PWD/src/simpleGenerators/waitForMessageGenerator.h \
	$$PWD/src/simpleGenerators/waitGamepadButtonGenerator.h \
	$$PWD/src/simpleGenerators/waitGamepadConnectGenerator.h \
	$$PWD/src/simpleGenerators/waitGamepadDisconnectGenerator.h \
	$$PWD/src/simpleGenerators/waitGamepadWheelGenerator.h \
	$$PWD/src/simpleGenerators/waitPadPressGenerator.h \
	$$PWD/src/simpleGenerators/fileWriteGenerator.h \
	$$PWD/src/simpleGenerators/removeFileGenerator.h \

SOURCES += \
	$$PWD/src/trikGeneratorPluginBase.cpp \
	$$PWD/src/trikMasterGeneratorBase.cpp \
	$$PWD/src/trikGeneratorCustomizer.cpp \
	$$PWD/src/trikGeneratorFactory.cpp \
	$$PWD/src/robotModel/trikGeneratorRobotModel.cpp \
	$$PWD/src/parts/trikDeviceVariables.cpp \
	$$PWD/src/converters/backgroundColorConverter.cpp \
	$$PWD/src/converters/ledColorConverter.cpp \
	$$PWD/src/simpleGenerators/detectGenerator.cpp \
	$$PWD/src/simpleGenerators/initCameraGenerator.cpp \
	$$PWD/src/simpleGenerators/ledGenerator.cpp \
	$$PWD/src/simpleGenerators/detectorToVariableGenerator.cpp \
	$$PWD/src/simpleGenerators/playToneGenerator.cpp \
	$$PWD/src/simpleGenerators/sadSmileGenerator.cpp \
	$$PWD/src/simpleGenerators/sayGenerator.cpp \
	$$PWD/src/simpleGenerators/systemGenerator.cpp \
	$$PWD/src/simpleGenerators/setBackgroundGenerator.cpp \
	$$PWD/src/simpleGenerators/smileGenerator.cpp \
	$$PWD/src/simpleGenerators/trikEnginesGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForInfraredSensorGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForMotionGenerator.cpp \
	$$PWD/src/simpleGenerators/drawLineGenerator.cpp \
	$$PWD/src/simpleGenerators/drawPixelGenerator.cpp \
	$$PWD/src/simpleGenerators/drawRectGenerator.cpp \
	$$PWD/src/simpleGenerators/drawEllipseGenerator.cpp \
	$$PWD/src/simpleGenerators/drawArcGenerator.cpp \
	$$PWD/src/simpleGenerators/setPainterWidthGenerator.cpp \
	$$PWD/src/simpleGenerators/setPainterColorGenerator.cpp \
	$$PWD/src/simpleGenerators/sendMessageGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForMessageGenerator.cpp \
	$$PWD/src/simpleGenerators/waitGamepadButtonGenerator.cpp \
	$$PWD/src/simpleGenerators/waitGamepadConnectGenerator.cpp \
	$$PWD/src/simpleGenerators/waitGamepadDisconnectGenerator.cpp \
	$$PWD/src/simpleGenerators/waitGamepadWheelGenerator.cpp \
	$$PWD/src/simpleGenerators/waitPadPressGenerator.cpp \
	$$PWD/src/simpleGenerators/fileWriteGenerator.cpp \
	$$PWD/src/simpleGenerators/removeFileGenerator.cpp \
