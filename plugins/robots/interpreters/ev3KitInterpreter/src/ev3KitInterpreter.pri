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

includes(plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/common/ev3Kit \
		plugins/robots/utils \
		qrtext \
		plugins/robots/interpeters/ev3KitInterpreter/src \
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
	$$PWD/ev3AdditionalPreferences.h \
	$$PWD/ev3KitInterpreterPlugin.h \
	$$PWD/ev3DisplayWidget.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/usbRealRobotModel.h \
	$$PWD/robotModel/real/bluetoothRealRobotModel.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/motor.h \
	$$PWD/robotModel/real/parts/button.h \
	$$PWD/robotModel/real/parts/led.h \
	$$PWD/robotModel/real/parts/ev3InputDevice.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/touchSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/rangeSensor.h \
	$$PWD/robotModel/real/parts/colorSensorFull.h \
	$$PWD/robotModel/real/parts/colorSensorPassive.h \
	$$PWD/robotModel/real/parts/colorSensorRed.h \
	$$PWD/robotModel/real/parts/colorSensorGreen.h \
	$$PWD/robotModel/real/parts/colorSensorBlue.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/robotModel/twoD/parts/twoDMotor.h \
	$$PWD/robotModel/twoD/parts/twoDLed.h \

SOURCES += \
	$$PWD/ev3AdditionalPreferences.cpp \
	$$PWD/ev3KitInterpreterPlugin.cpp \
	$$PWD/ev3DisplayWidget.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/usbRealRobotModel.cpp \
	$$PWD/robotModel/real/bluetoothRealRobotModel.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
	$$PWD/robotModel/real/parts/button.cpp \
	$$PWD/robotModel/real/parts/led.cpp \
	$$PWD/robotModel/real/parts/ev3InputDevice.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/touchSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/rangeSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensorFull.cpp \
	$$PWD/robotModel/real/parts/colorSensorPassive.cpp \
	$$PWD/robotModel/real/parts/colorSensorRed.cpp \
	$$PWD/robotModel/real/parts/colorSensorGreen.cpp \
	$$PWD/robotModel/real/parts/colorSensorBlue.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/robotModel/twoD/parts/twoDMotor.cpp \
	$$PWD/robotModel/twoD/parts/twoDLed.cpp \

FORMS += \
	$$PWD/ev3AdditionalPreferences.ui \
	$$PWD/ev3DisplayWidget.ui \

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3KitInterpreter_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/ev3KitInterpreter_fr.ts \

RESOURCES += \
	$$PWD/../ev3KitInterpreter.qrc \
