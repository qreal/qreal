# Copyright 2015 CyberTech Labs Ltd.
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
		)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-trik-kit qextserialport qslog \
		)

HEADERS += \
	$$PWD/include/trikKitInterpreterCommon/declSpec.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/display.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/speaker.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/button.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/powerMotor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/servoMotor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/encoderSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/touchSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/lightSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/infraredSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/sonarSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/led.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/lineSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/motionSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/objectSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/colorSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/gyroscope.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/accelerometer.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/shell.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/gamepadButton.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/gamepadConnectionIndicator.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/gamepadPad.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/gamepadPadPressSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/real/parts/gamepadWheel.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDInfraredSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDLed.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDLineSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDObjectSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDColorSensor.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDShell.h \
	$$PWD/include/trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h \
	$$PWD/include/trikKitInterpreterCommon/trikAdditionalPreferences.h \
	$$PWD/include/trikKitInterpreterCommon/trikDisplayWidget.h \
	$$PWD/include/trikKitInterpreterCommon/trikKitInterpreterPluginBase.h \

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
	$$PWD/trikKitInterpreterCommon.qrc \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/trikKitInterpreterCommon_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/trikKitInterpreterCommon_fr.ts \

OTHER_FILES += \
	$$PWD/trikDefaultSettings.ini \
