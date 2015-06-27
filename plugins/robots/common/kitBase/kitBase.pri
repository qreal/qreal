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

QT += widgets xml

links(qrkernel qslog qrutils qrtext qrgui-preferences-dialog robots-utils)
includes(qrtext plugins/robots/utils)

DEFINES += ROBOTS_KIT_BASE_LIBRARY

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/kitBase_ru.ts

HEADERS += \
	$$PWD/include/kitBase/kitBaseDeclSpec.h \
	$$PWD/include/kitBase/kitPluginInterface.h \
	$$PWD/include/kitBase/kitPluginConfigurator.h \
	$$PWD/include/kitBase/additionalPreferences.h \
	$$PWD/include/kitBase/devicesConfigurationProvider.h \
	$$PWD/include/kitBase/devicesConfigurationWidget.h \
	$$PWD/include/kitBase/eventsForKitPluginInterface.h \
	$$PWD/include/kitBase/interpreterControlInterface.h \
	$$PWD/include/kitBase/readOnly.h \
	$$PWD/include/kitBase/blocksBase/robotsBlock.h \
	$$PWD/include/kitBase/blocksBase/blocksFactoryInterface.h \
	$$PWD/include/kitBase/blocksBase/commonBlocksFactory.h \
	$$PWD/include/kitBase/blocksBase/common/clearEncoderBlock.h \
	$$PWD/include/kitBase/blocksBase/common/engineCommandBlock.h \
	$$PWD/include/kitBase/blocksBase/common/enginesStopBlock.h \
	$$PWD/include/kitBase/blocksBase/common/deviceBlock.h \
	$$PWD/include/kitBase/blocksBase/common/displayBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForAccelerometerBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForColorBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForColorIntensityBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForColorSensorBlockBase.h \
	$$PWD/include/kitBase/blocksBase/common/waitForEncoderBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForGyroscopeSensorBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForLightSensorBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForSensorBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForSonarDistanceBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForSoundSensorBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForTouchSensorBlock.h \
	$$PWD/include/kitBase/blocksBase/common/waitForButtonBlock.h \
	$$PWD/include/kitBase/blocksBase/common/getButtonCodeBlock.h \
	$$PWD/include/kitBase/robotModel/configuration.h \
	$$PWD/include/kitBase/robotModel/commonRobotModel.h \
	$$PWD/include/kitBase/robotModel/configurationInterface.h \
	$$PWD/include/kitBase/robotModel/portInfo.h \
	$$PWD/include/kitBase/robotModel/deviceInfo.h \
	$$PWD/include/kitBase/robotModel/direction.h \
	$$PWD/include/kitBase/robotModel/robotModelManagerInterface.h \
	$$PWD/include/kitBase/robotModel/robotModelInterface.h \
	$$PWD/include/kitBase/robotModel/robotModelUtils.h \
	$$PWD/include/kitBase/robotModel/robotParts/speaker.h \
	$$PWD/include/kitBase/robotModel/robotParts/motor.h \
	$$PWD/include/kitBase/robotModel/robotParts/display.h \
	$$PWD/include/kitBase/robotModel/robotParts/button.h \
	$$PWD/include/kitBase/robotModel/robotParts/abstractSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/accelerometerSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/colorSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/colorSensorPassive.h \
	$$PWD/include/kitBase/robotModel/robotParts/colorSensorFull.h \
	$$PWD/include/kitBase/robotModel/robotParts/colorSensorRed.h \
	$$PWD/include/kitBase/robotModel/robotParts/colorSensorGreen.h \
	$$PWD/include/kitBase/robotModel/robotParts/colorSensorBlue.h \
	$$PWD/include/kitBase/robotModel/robotParts/encoderSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/gyroscopeSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/lightSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/device.h \
	$$PWD/include/kitBase/robotModel/robotParts/rangeSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/scalarSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/soundSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/touchSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/vectorSensor.h \
	$$PWD/include/kitBase/robotModel/robotParts/shell.h \

SOURCES += \
	$$PWD/src/devicesConfigurationProvider.cpp \
	$$PWD/src/devicesConfigurationWidget.cpp \
	$$PWD/src/blocksBase/robotsBlock.cpp \
	$$PWD/src/blocksBase/commonBlocksFactory.cpp \
	$$PWD/src/blocksBase/common/clearEncoderBlock.cpp \
	$$PWD/src/blocksBase/common/engineCommandBlock.cpp \
	$$PWD/src/blocksBase/common/enginesStopBlock.cpp \
	$$PWD/src/blocksBase/common/waitBlock.cpp \
	$$PWD/src/blocksBase/common/waitForAccelerometerBlock.cpp \
	$$PWD/src/blocksBase/common/waitForColorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForColorIntensityBlock.cpp \
	$$PWD/src/blocksBase/common/waitForColorSensorBlockBase.cpp \
	$$PWD/src/blocksBase/common/waitForEncoderBlock.cpp \
	$$PWD/src/blocksBase/common/waitForGyroscopeSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForLightSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForSonarDistanceBlock.cpp \
	$$PWD/src/blocksBase/common/waitForSoundSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForTouchSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForButtonBlock.cpp \
	$$PWD/src/blocksBase/common/getButtonCodeBlock.cpp \
	$$PWD/src/robotModel/configuration.cpp \
	$$PWD/src/robotModel/commonRobotModel.cpp \
	$$PWD/src/robotModel/robotModelUtils.cpp \
	$$PWD/src/robotModel/portInfo.cpp \
	$$PWD/src/robotModel/deviceInfo.cpp \
	$$PWD/src/robotModel/robotParts/speaker.cpp \
	$$PWD/src/robotModel/robotParts/motor.cpp \
	$$PWD/src/robotModel/robotParts/display.cpp \
	$$PWD/src/robotModel/robotParts/button.cpp \
	$$PWD/src/robotModel/robotParts/abstractSensor.cpp \
	$$PWD/src/robotModel/robotParts/accelerometerSensor.cpp \
	$$PWD/src/robotModel/robotParts/colorSensor.cpp \
	$$PWD/src/robotModel/robotParts/colorSensorPassive.cpp \
	$$PWD/src/robotModel/robotParts/colorSensorFull.cpp \
	$$PWD/src/robotModel/robotParts/colorSensorRed.cpp \
	$$PWD/src/robotModel/robotParts/colorSensorGreen.cpp \
	$$PWD/src/robotModel/robotParts/colorSensorBlue.cpp \
	$$PWD/src/robotModel/robotParts/encoderSensor.cpp \
	$$PWD/src/robotModel/robotParts/gyroscopeSensor.cpp \
	$$PWD/src/robotModel/robotParts/lightSensor.cpp \
	$$PWD/src/robotModel/robotParts/device.cpp \
	$$PWD/src/robotModel/robotParts/rangeSensor.cpp \
	$$PWD/src/robotModel/robotParts/scalarSensor.cpp \
	$$PWD/src/robotModel/robotParts/soundSensor.cpp \
	$$PWD/src/robotModel/robotParts/touchSensor.cpp \
	$$PWD/src/robotModel/robotParts/vectorSensor.cpp \
	$$PWD/src/robotModel/robotParts/shell.cpp \
