# Copyright 2015-2016 CyberTech Labs Ltd.
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

includes(plugins/robots/core/kitBase \
		plugins/robots/core/twoDModel \
		plugins/robots/trik/trikCommon \
		plugins/robots/utils \
		qrtext \
		)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-trik-kit qextserialport qslog \
		)

HEADERS += \
	$$PWD/include/trikInterpreterCommon/declSpec.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/display.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/speaker.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/button.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/powerMotor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/servoMotor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/encoderSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/touchSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/lightSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/infraredSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/sonarSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/led.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/lineSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/motionSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/objectSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/colorSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/gyroscope.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/accelerometer.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/shell.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/gamepadButton.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/gamepadConnectionIndicator.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/gamepadPad.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/gamepadPadPressSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/real/parts/gamepadWheel.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDInfraredSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDLed.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDLineSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDObjectSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDColorSensor.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/parts/twoDShell.h \
	$$PWD/include/trikInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h \
	$$PWD/include/trikInterpreterCommon/trikAdditionalPreferences.h \
	$$PWD/include/trikInterpreterCommon/trikDisplayWidget.h \
	$$PWD/include/trikInterpreterCommon/trikKitInterpreterPluginBase.h \

SOURCES += \
	$$PWD/src/robotModel/real/parts/display.cpp \
	$$PWD/src/robotModel/real/parts/speaker.cpp \
	$$PWD/src/robotModel/real/parts/button.cpp \
	$$PWD/src/robotModel/real/parts/powerMotor.cpp \
	$$PWD/src/robotModel/real/parts/servoMotor.cpp \
	$$PWD/src/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/src/robotModel/real/parts/touchSensor.cpp \
	$$PWD/src/robotModel/real/parts/lightSensor.cpp \
	$$PWD/src/robotModel/real/parts/infraredSensor.cpp \
	$$PWD/src/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/src/robotModel/real/parts/led.cpp \
	$$PWD/src/robotModel/real/parts/lineSensor.cpp \
	$$PWD/src/robotModel/real/parts/objectSensor.cpp \
	$$PWD/src/robotModel/real/parts/colorSensor.cpp \
	$$PWD/src/robotModel/real/parts/motionSensor.cpp \
	$$PWD/src/robotModel/real/parts/gyroscope.cpp \
	$$PWD/src/robotModel/real/parts/accelerometer.cpp \
	$$PWD/src/robotModel/real/parts/shell.cpp \
	$$PWD/src/robotModel/real/parts/gamepadButton.cpp \
	$$PWD/src/robotModel/real/parts/gamepadConnectionIndicator.cpp \
	$$PWD/src/robotModel/real/parts/gamepadPad.cpp \
	$$PWD/src/robotModel/real/parts/gamepadPadPressSensor.cpp \
	$$PWD/src/robotModel/real/parts/gamepadWheel.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDInfraredSensor.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDLineSensor.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDObjectSensor.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDColorSensor.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDLed.cpp \
	$$PWD/src/robotModel/twoD/parts/twoDShell.cpp \
	$$PWD/src/robotModel/twoD/trikTwoDRobotModel.cpp \
	$$PWD/src/trikAdditionalPreferences.cpp \
	$$PWD/src/trikDisplayWidget.cpp \
	$$PWD/src/trikKitInterpreterPluginBase.cpp \

FORMS += \
	$$PWD/src/trikAdditionalPreferences.ui \
	$$PWD/src/trikDisplayWidget.ui \

RESOURCES += \
	$$PWD/trikInterpreterCommon.qrc \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/trikInterpreterCommon_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/trikInterpreterCommon_fr.ts \

OTHER_FILES += \
	$$PWD/trikDefaultSettings.ini \
