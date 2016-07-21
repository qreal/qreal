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

includes(plugins/robots/core/kitBase \
		plugins/robots/core/twoDModel \
		plugins/robots/ev3/ev3Common \
		plugins/robots/utils \
		qrtext \
		plugins/robots/ev3/ev3Interpreter/src \
		plugins/robots/thirdparty/libusb-1.0.19/include/libusb-1.0 \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-ev3-kit robots-2d-model qextserialport qslog \
)

win32 {
	LIBS += -llibusb-1.0
}

unix:!macx {
	LIBS += -L /usr/bin -lusb-1.0
}

macx {
	LIBS += -lusb-1.0
}

HEADERS += \
	$$PWD/src/ev3AdditionalPreferences.h \
	$$PWD/src/ev3KitInterpreterPlugin.h \
	$$PWD/src/ev3DisplayWidget.h \
	$$PWD/src/robotModel/real/realRobotModel.h \
	$$PWD/src/robotModel/real/usbRealRobotModel.h \
	$$PWD/src/robotModel/real/bluetoothRealRobotModel.h \
	$$PWD/src/robotModel/real/parts/display.h \
	$$PWD/src/robotModel/real/parts/speaker.h \
	$$PWD/src/robotModel/real/parts/motor.h \
	$$PWD/src/robotModel/real/parts/button.h \
	$$PWD/src/robotModel/real/parts/led.h \
	$$PWD/src/robotModel/real/parts/ev3InputDevice.h \
	$$PWD/src/robotModel/real/parts/encoderSensor.h \
	$$PWD/src/robotModel/real/parts/touchSensor.h \
	$$PWD/src/robotModel/real/parts/lightSensor.h \
	$$PWD/src/robotModel/real/parts/rangeSensor.h \
	$$PWD/src/robotModel/real/parts/colorSensorFull.h \
	$$PWD/src/robotModel/real/parts/colorSensorPassive.h \
	$$PWD/src/robotModel/real/parts/colorSensorRed.h \
	$$PWD/src/robotModel/real/parts/colorSensorGreen.h \
	$$PWD/src/robotModel/real/parts/colorSensorBlue.h \
	$$PWD/src/robotModel/twoD/twoDRobotModel.h \
	$$PWD/src/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/src/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/src/robotModel/twoD/parts/twoDMotor.h \
	$$PWD/src/robotModel/twoD/parts/twoDLed.h \

SOURCES += \
	$$PWD/src/ev3AdditionalPreferences.cpp \
	$$PWD/src/ev3KitInterpreterPlugin.cpp \
	$$PWD/src/ev3DisplayWidget.cpp \
	$$PWD/src/robotModel/real/realRobotModel.cpp \
	$$PWD/src/robotModel/real/usbRealRobotModel.cpp \
	$$PWD/src/robotModel/real/bluetoothRealRobotModel.cpp \
	$$PWD/src/robotModel/real/parts/display.cpp \
	$$PWD/src/robotModel/real/parts/speaker.cpp \
	$$PWD/src/robotModel/real/parts/motor.cpp \
	$$PWD/src/robotModel/real/parts/button.cpp \
	$$PWD/src/robotModel/real/parts/led.cpp \
	$$PWD/src/robotModel/real/parts/ev3InputDevice.cpp \
	$$PWD/src/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/src/robotModel/real/parts/touchSensor.cpp \
	$$PWD/src/robotModel/real/parts/lightSensor.cpp \
	$$PWD/src/robotModel/real/parts/rangeSensor.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorFull.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorPassive.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorRed.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorGreen.cpp \
	$$PWD/src/robotModel/real/parts/colorSensorBlue.cpp \
	$$PWD/src/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDMotor.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDLed.cpp \

FORMS += \
	$$PWD/src/ev3AdditionalPreferences.ui \
	$$PWD/src/ev3DisplayWidget.ui \

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3Interpreter_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/ev3Interpreter_fr.ts \

RESOURCES += \
	$$PWD/ev3Interpreter.qrc \
