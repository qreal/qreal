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

QT += widgets network

includes(plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
		plugins/robots/interpreters/trikKitInterpreter/src)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-trik-kit qextserialport qslog \
		)

HEADERS += \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/button.h \
	$$PWD/robotModel/real/parts/powerMotor.h \
	$$PWD/robotModel/real/parts/servoMotor.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/infraredSensor.h \
	$$PWD/robotModel/real/parts/sonarSensor.h \
	$$PWD/robotModel/real/parts/led.h \
	$$PWD/robotModel/real/parts/lineSensor.h \
	$$PWD/robotModel/real/parts/motionSensor.h \
	$$PWD/robotModel/real/parts/objectSensor.h \
	$$PWD/robotModel/real/parts/colorSensor.h \
	$$PWD/robotModel/real/parts/gyroscope.h \
	$$PWD/robotModel/real/parts/accelerometer.h \
	$$PWD/robotModel/real/parts/shell.h \
	$$PWD/robotModel/real/parts/gamepadButton.h \
	$$PWD/robotModel/real/parts/gamepadConnectionIndicator.h \
	$$PWD/robotModel/real/parts/gamepadPad.h \
	$$PWD/robotModel/real/parts/gamepadPadPressSensor.h \
	$$PWD/robotModel/real/parts/gamepadWheel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/robotModel/twoD/parts/twoDInfraredSensor.h \
	$$PWD/robotModel/twoD/parts/twoDLed.h \
	$$PWD/robotModel/twoD/parts/twoDLineSensor.h \
	$$PWD/robotModel/twoD/parts/twoDObjectSensor.h \
	$$PWD/robotModel/twoD/parts/twoDColorSensor.h \
	$$PWD/robotModel/twoD/parts/twoDShell.h \
	$$PWD/robotModel/real/trikV62RealRobotModel.h \
	$$PWD/robotModel/twoD/trikV62TwoDRobotModel.h \
	$$PWD/trikV62AdditionalPreferences.h \
	$$PWD/trikV62KitInterpreterPlugin.h \
	$$PWD/trikV62DisplayWidget.h \

SOURCES += \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/button.cpp \
	$$PWD/robotModel/real/parts/powerMotor.cpp \
	$$PWD/robotModel/real/parts/servoMotor.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/infraredSensor.cpp \
	$$PWD/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/robotModel/real/parts/led.cpp \
	$$PWD/robotModel/real/parts/lineSensor.cpp \
	$$PWD/robotModel/real/parts/objectSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensor.cpp \
	$$PWD/robotModel/real/parts/motionSensor.cpp \
	$$PWD/robotModel/real/parts/gyroscope.cpp \
	$$PWD/robotModel/real/parts/accelerometer.cpp \
	$$PWD/robotModel/real/parts/shell.cpp \
	$$PWD/robotModel/real/parts/gamepadButton.cpp \
	$$PWD/robotModel/real/parts/gamepadConnectionIndicator.cpp \
	$$PWD/robotModel/real/parts/gamepadPad.cpp \
	$$PWD/robotModel/real/parts/gamepadPadPressSensor.cpp \
	$$PWD/robotModel/real/parts/gamepadWheel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/robotModel/twoD/parts/twoDInfraredSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDLineSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDObjectSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDColorSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDLed.cpp \
	$$PWD/robotModel/twoD/parts/twoDShell.cpp \
	$$PWD/trikV62AdditionalPreferences.cpp \
	$$PWD/trikV62DisplayWidget.cpp \
	$$PWD/trikV62KitInterpreterPlugin.cpp \
	$$PWD/robotModel/real/trikV62RealRobotModel.cpp \
	$$PWD/robotModel/twoD/trikV62TwoDRobotModel.cpp \

FORMS += \
	$$PWD/trikV62AdditionalPreferences.ui \
	$$PWD/trikV62DisplayWidget.ui

RESOURCES += \
	$$PWD/../trikV62KitInterpreter.qrc \

TRANSLATIONS += \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikV62KitInterpreter_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikV62KitInterpreter_fr.ts \
