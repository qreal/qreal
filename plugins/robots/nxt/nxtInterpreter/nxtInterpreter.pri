# Copyright 2013-2016 CyberTech Labs Ltd.
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

includes(plugins/robots/core/kitBase \
		plugins/robots/core/twoDModel \
		plugins/robots/nxt/nxtCommon \
		plugins/robots/utils \
		qrtext \
		plugins/robots/nxt/nxtInterpreter/src \
)

HEADERS += \
	$$PWD/src/commandConstants.h \
	$$PWD/src/nxtAdditionalPreferences.h \
	$$PWD/src/nxtDisplayWidget.h \
	$$PWD/src/nxtKitInterpreterPlugin.h \
	$$PWD/src/robotModel/real/realRobotModel.h \
	$$PWD/src/robotModel/real/usbRealRobotModel.h \
	$$PWD/src/robotModel/real/bluetoothRealRobotModel.h \
	$$PWD/src/robotModel/real/parts/nxtInputDevice.h \
	$$PWD/src/robotModel/real/parts/display.h \
	$$PWD/src/robotModel/real/parts/speaker.h \
	$$PWD/src/robotModel/real/parts/button.h \
	$$PWD/src/robotModel/real/parts/motor.h \
	$$PWD/src/robotModel/real/parts/touchSensor.h \
	$$PWD/src/robotModel/real/parts/colorSensorFull.h \
	$$PWD/src/robotModel/real/parts/colorSensorRed.h \
	$$PWD/src/robotModel/real/parts/colorSensorGreen.h \
	$$PWD/src/robotModel/real/parts/colorSensorBlue.h \
	$$PWD/src/robotModel/real/parts/colorSensorPassive.h \
	$$PWD/src/robotModel/real/parts/encoderSensor.h \
	$$PWD/src/robotModel/real/parts/gyroscopeSensor.h \
	$$PWD/src/robotModel/real/parts/lightSensor.h \
	$$PWD/src/robotModel/real/parts/sonarSensor.h \
	$$PWD/src/robotModel/real/parts/soundSensor.h \
	$$PWD/src/robotModel/real/parts/implementations/colorSensorImpl.h \
	$$PWD/src/robotModel/twoD/twoDRobotModel.h \
	$$PWD/src/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/src/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/src/robotModel/twoD/parts/twoDMotor.h \

SOURCES += \
	$$PWD/src/nxtAdditionalPreferences.cpp \
	$$PWD/src/nxtDisplayWidget.cpp \
	$$PWD/src/nxtKitInterpreterPlugin.cpp \
	$$PWD/src/robotModel/real/realRobotModel.cpp \
	$$PWD/src/robotModel/real/usbRealRobotModel.cpp \
	$$PWD/src/robotModel/real/bluetoothRealRobotModel.cpp \
	$$PWD/src/robotModel/real/parts/nxtInputDevice.cpp \
	$$PWD/src/robotModel/real/parts/display.cpp \
	$$PWD/src/robotModel/real/parts/speaker.cpp \
	$$PWD/src/robotModel/real/parts/button.cpp \
	$$PWD/src/robotModel/real/parts/motor.cpp \
	$$PWD/src/robotModel/real/parts/touchSensor.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorFull.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorRed.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorGreen.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorBlue.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorPassive.cpp \
	$$PWD/src/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/src/robotModel/real/parts/gyroscopeSensor.cpp \
	$$PWD/src/robotModel/real/parts/lightSensor.cpp \
	$$PWD/src/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/src/robotModel/real/parts/soundSensor.cpp \
	$$PWD/src/robotModel/real/parts/implementations/colorSensorImpl.cpp \
	$$PWD/src/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDMotor.cpp \

FORMS += \
	$$PWD/src/nxtAdditionalPreferences.ui \
	$$PWD/src/nxtDisplayWidget.ui \

include($$PWD/src/communication/communication.pri)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtInterpreter_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/nxtInterpreter_fr.ts \

RESOURCES += $$PWD/nxtInterpreter.qrc
