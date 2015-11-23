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

QT += widgets

includes(plugins/robots/common/trikKit \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base qextserialport qslog \
)

HEADERS += \
	$$PWD/include/trikKit/robotModel/trikRobotModelBase.h \
	$$PWD/include/trikKit/robotModel/trikRobotModelV6.h \
	$$PWD/include/trikKit/robotModel/trikRobotModelV62.h \
	$$PWD/include/trikKit/robotModel/parts/trikColorSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikDisplay.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadButton.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadConnectionIndicator.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadPad.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadPadPressSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadWheel.h \
	$$PWD/include/trikKit/robotModel/parts/trikSpeaker.h \
	$$PWD/include/trikKit/robotModel/parts/trikPowerMotor.h \
	$$PWD/include/trikKit/robotModel/parts/trikServoMotor.h \
	$$PWD/include/trikKit/robotModel/parts/trikInfraredSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikLed.h \
	$$PWD/include/trikKit/robotModel/parts/trikLineSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikMotionSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikSonarSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikObjectSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikShell.h \
	$$PWD/include/trikKit/blocks/trikBlocksFactoryBase.h \
	$$PWD/include/trikKit/blocks/trikV6BlocksFactory.h \
	$$PWD/include/trikKit/blocks/trikV62BlocksFactory.h \
	$$PWD/src/blocks/details/setBackgroundBlock.h \
	$$PWD/src/blocks/details/smileBlock.h \
	$$PWD/src/blocks/details/trikEnginesForwardBlock.h \
	$$PWD/src/blocks/details/trikEnginesBackwardBlock.h \
	$$PWD/src/blocks/details/waitForMotionBlock.h \
	$$PWD/src/blocks/details/speakerBlock.h \
	$$PWD/src/blocks/details/ledBlock.h \
	$$PWD/src/blocks/details/sayBlock.h \
	$$PWD/src/blocks/details/systemCommandBlock.h \
	$$PWD/src/blocks/details/waitGamepadButtonBlock.h \
	$$PWD/src/blocks/details/waitGamepadConnectBlock.h \
	$$PWD/src/blocks/details/waitGamepadDisconnectBlock.h \
	$$PWD/src/blocks/details/waitGamepadWheelBlock.h \
	$$PWD/src/blocks/details/waitPadPressBlock.h \
	$$PWD/src/blocks/details/drawLineBlock.h \
	$$PWD/src/blocks/details/drawPixelBlock.h \
	$$PWD/src/blocks/details/drawRectBlock.h \
	$$PWD/src/blocks/details/drawEllipseBlock.h \
	$$PWD/src/blocks/details/drawArcBlock.h \
	$$PWD/src/blocks/details/setPainterWidthBlock.h \
	$$PWD/src/blocks/details/setPainterColorBlock.h \
	$$PWD/src/blocks/details/initCameraBlock.h \
	$$PWD/src/blocks/details/detectLineBlock.h \
	$$PWD/src/blocks/details/lineDetectorToVariable.h \
	$$PWD/src/blocks/details/initVideoStreamingBlock.h \
	$$PWD/src/blocks/details/writeToFileBlock.h \
	$$PWD/src/blocks/details/removeFileBlock.h \

SOURCES += \
	$$PWD/src/robotModel/trikRobotModelBase.cpp \
	$$PWD/src/robotModel/trikRobotModelV6.cpp \
	$$PWD/src/robotModel/trikRobotModelV62.cpp \
	$$PWD/src/robotModel/parts/trikColorSensor.cpp \
	$$PWD/src/robotModel/parts/trikDisplay.cpp \
	$$PWD/src/robotModel/parts/trikGamepadButton.cpp \
	$$PWD/src/robotModel/parts/trikGamepadConnectionIndicator.cpp \
	$$PWD/src/robotModel/parts/trikGamepadPad.cpp \
	$$PWD/src/robotModel/parts/trikGamepadPadPressSensor.cpp \
	$$PWD/src/robotModel/parts/trikGamepadWheel.cpp \
	$$PWD/src/robotModel/parts/trikInfraredSensor.cpp \
	$$PWD/src/robotModel/parts/trikLed.cpp \
	$$PWD/src/robotModel/parts/trikLineSensor.cpp \
	$$PWD/src/robotModel/parts/trikMotionSensor.cpp \
	$$PWD/src/robotModel/parts/trikObjectSensor.cpp \
	$$PWD/src/robotModel/parts/trikPowerMotor.cpp \
	$$PWD/src/robotModel/parts/trikServoMotor.cpp \
	$$PWD/src/robotModel/parts/trikSonarSensor.cpp \
	$$PWD/src/robotModel/parts/trikSpeaker.cpp \
	$$PWD/src/robotModel/parts/trikShell.cpp \
	$$PWD/src/blocks/trikBlocksFactoryBase.cpp \
	$$PWD/src/blocks/trikV6BlocksFactory.cpp \
	$$PWD/src/blocks/trikV62BlocksFactory.cpp \
	$$PWD/src/blocks/details/setBackgroundBlock.cpp \
	$$PWD/src/blocks/details/smileBlock.cpp \
	$$PWD/src/blocks/details/trikEnginesForwardBlock.cpp \
	$$PWD/src/blocks/details/trikEnginesBackwardBlock.cpp \
	$$PWD/src/blocks/details/waitForMotionBlock.cpp \
	$$PWD/src/blocks/details/speakerBlock.cpp \
	$$PWD/src/blocks/details/ledBlock.cpp \
	$$PWD/src/blocks/details/sayBlock.cpp \
	$$PWD/src/blocks/details/systemCommandBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadButtonBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadConnectBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadDisconnectBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadWheelBlock.cpp \
	$$PWD/src/blocks/details/waitPadPressBlock.cpp \
	$$PWD/src/blocks/details/drawLineBlock.cpp \
	$$PWD/src/blocks/details/drawPixelBlock.cpp \
	$$PWD/src/blocks/details/drawRectBlock.cpp \
	$$PWD/src/blocks/details/drawEllipseBlock.cpp \
	$$PWD/src/blocks/details/drawArcBlock.cpp \
	$$PWD/src/blocks/details/setPainterWidthBlock.cpp \
	$$PWD/src/blocks/details/setPainterColorBlock.cpp \
	$$PWD/src/blocks/details/initCameraBlock.cpp \
	$$PWD/src/blocks/details/detectLineBlock.cpp \
	$$PWD/src/blocks/details/lineDetectorToVariable.cpp \
	$$PWD/src/blocks/details/initVideoStreamingBlock.cpp \
	$$PWD/src/blocks/details/writeToFileBlock.cpp \
	$$PWD/src/blocks/details/removeFileBlock.cpp \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/trikKit_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/trikKit_fr.ts \
