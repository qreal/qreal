# Copyright 2014-2016 CyberTech Labs Ltd.
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

includes(plugins/robots/trik/trikCommon \
		plugins/robots/core/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base qextserialport qslog \
)

HEADERS += \
	$$PWD/include/trikCommon/robotModel/trikRobotModelBase.h \
	$$PWD/include/trikCommon/robotModel/trikRobotModelV6.h \
	$$PWD/include/trikCommon/robotModel/trikRobotModelV62.h \
	$$PWD/include/trikCommon/robotModel/parts/trikColorSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikDisplay.h \
	$$PWD/include/trikCommon/robotModel/parts/trikGamepadButton.h \
	$$PWD/include/trikCommon/robotModel/parts/trikGamepadConnectionIndicator.h \
	$$PWD/include/trikCommon/robotModel/parts/trikGamepadPad.h \
	$$PWD/include/trikCommon/robotModel/parts/trikGamepadPadPressSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikGamepadWheel.h \
	$$PWD/include/trikCommon/robotModel/parts/trikSpeaker.h \
	$$PWD/include/trikCommon/robotModel/parts/trikPowerMotor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikServoMotor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikInfraredSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikLed.h \
	$$PWD/include/trikCommon/robotModel/parts/trikLineSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikMotionSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikSonarSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikObjectSensor.h \
	$$PWD/include/trikCommon/robotModel/parts/trikShell.h \
	$$PWD/include/trikCommon/blocks/trikBlocksFactoryBase.h \
	$$PWD/include/trikCommon/blocks/trikV6BlocksFactory.h \
	$$PWD/include/trikCommon/blocks/trikV62BlocksFactory.h \
	$$PWD/include/trikCommon/robotCommunication/networkCommunicationErrorReporter.h \
	$$PWD/include/trikCommon/robotCommunication/runProgramProtocol.h \
	$$PWD/include/trikCommon/robotCommunication/stopRobotProtocol.h \
	$$PWD/include/trikCommon/robotCommunication/tcpRobotCommunicator.h \
	$$PWD/include/trikCommon/robotCommunication/tcpRobotCommunicatorInterface.h \
	$$PWD/include/trikCommon/robotCommunication/uploadProgramProtocol.h \
	$$PWD/../runtimeSupport/requiredVersion.h \
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
	$$PWD/src/robotCommunication/protocol.h \
	$$PWD/src/robotCommunication/guardSignalGenerator.h \
	$$PWD/src/robotCommunication/tcpConnectionHandler.h \
	$$PWD/src/robotCommunication/tcpRobotCommunicatorWorker.h \


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
	$$PWD/src/robotCommunication/networkCommunicationErrorReporter.cpp \
	$$PWD/src/robotCommunication/protocol.cpp \
	$$PWD/src/robotCommunication/runProgramProtocol.cpp \
	$$PWD/src/robotCommunication/stopRobotProtocol.cpp \
	$$PWD/src/robotCommunication/tcpRobotCommunicator.cpp \
	$$PWD/src/robotCommunication/tcpConnectionHandler.cpp \
	$$PWD/src/robotCommunication/tcpRobotCommunicatorWorker.cpp \
	$$PWD/src/robotCommunication/uploadProgramProtocol.cpp \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/trikCommon_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/trikCommon_fr.ts \
