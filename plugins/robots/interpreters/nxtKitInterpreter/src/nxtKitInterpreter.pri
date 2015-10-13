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

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-nxt-kit qextserialport qslog \
		)

includes(plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/common/nxtKit \
		plugins/robots/utils \
		qrtext \
		plugins/robots/interpreters/nxtKitInterpreter/src \
)

HEADERS += \
	$$PWD/commandConstants.h \
	$$PWD/nxtAdditionalPreferences.h \
	$$PWD/nxtDisplayWidget.h \
	$$PWD/nxtKitInterpreterPlugin.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/usbRealRobotModel.h \
	$$PWD/robotModel/real/bluetoothRealRobotModel.h \
	$$PWD/robotModel/real/parts/nxtInputDevice.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/button.h \
	$$PWD/robotModel/real/parts/motor.h \
	$$PWD/robotModel/real/parts/touchSensor.h \
	$$PWD/robotModel/real/parts/colorSensorFull.h \
	$$PWD/robotModel/real/parts/colorSensorRed.h \
	$$PWD/robotModel/real/parts/colorSensorGreen.h \
	$$PWD/robotModel/real/parts/colorSensorBlue.h \
	$$PWD/robotModel/real/parts/colorSensorPassive.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/gyroscopeSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/sonarSensor.h \
	$$PWD/robotModel/real/parts/soundSensor.h \
	$$PWD/robotModel/real/parts/implementations/colorSensorImpl.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/robotModel/twoD/parts/twoDMotor.h \

SOURCES += \
	$$PWD/nxtAdditionalPreferences.cpp \
	$$PWD/nxtDisplayWidget.cpp \
	$$PWD/nxtKitInterpreterPlugin.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/usbRealRobotModel.cpp \
	$$PWD/robotModel/real/bluetoothRealRobotModel.cpp \
	$$PWD/robotModel/real/parts/nxtInputDevice.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/button.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
	$$PWD/robotModel/real/parts/touchSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensorFull.cpp \
	$$PWD/robotModel/real/parts/colorSensorRed.cpp \
	$$PWD/robotModel/real/parts/colorSensorGreen.cpp \
	$$PWD/robotModel/real/parts/colorSensorBlue.cpp \
	$$PWD/robotModel/real/parts/colorSensorPassive.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/gyroscopeSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/robotModel/real/parts/soundSensor.cpp \
	$$PWD/robotModel/real/parts/implementations/colorSensorImpl.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/robotModel/twoD/parts/twoDMotor.cpp \

FORMS += \
	$$PWD/nxtAdditionalPreferences.ui \
	$$PWD/nxtDisplayWidget.ui \

include($$PWD/communication/communication.pri)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtKitInterpreter_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/nxtKitInterpreter_fr.ts \

RESOURCES += $$PWD/../nxtKitInterpreter.qrc
