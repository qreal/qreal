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
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-ev3-kit qextserialport \
)

HEADERS += \
	$$PWD/commandConstants.h \
	$$PWD/ev3AdditionalPreferences.h \
	$$PWD/ev3KitInterpreterPlugin.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/motor.h \
	$$PWD/robotModel/real/parts/button.h \
	$$PWD/robotModel/real/parts/ev3InputDevice.h \
	$$PWD/robotModel/real/parts/touchSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/rangeSensor.h \
	$$PWD/robotModel/real/parts/colorSensorFull.h \
	$$PWD/robotModel/real/parts/implementations/colorSensorImpl.h \

SOURCES += \
	$$PWD/ev3AdditionalPreferences.cpp \
	$$PWD/ev3KitInterpreterPlugin.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
	$$PWD/robotModel/real/parts/button.cpp \
	$$PWD/robotModel/real/parts/ev3InputDevice.cpp \
	$$PWD/robotModel/real/parts/touchSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/rangeSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensorFull.cpp \
	$$PWD/robotModel/real/parts/implementations/colorSensorImpl.cpp \

FORMS += \
	$$PWD/ev3AdditionalPreferences.ui \

include($$PWD/communication/communication.pri)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3KitInterpreter_ru.ts
