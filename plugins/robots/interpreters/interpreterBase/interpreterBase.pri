QT += widgets xml

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrutils -lutils

DEFINES += ROBOTS_INTERPRETER_BASE_LIBRARY

TRANSLATIONS = interpreterBase_ru.ts

SOURCES += \
	$$PWD/src/devicesConfigurationProvider.cpp \
	$$PWD/src/blocksBase/block.cpp \
	$$PWD/src/blocksBase/commonBlocksFactory.cpp \
	$$PWD/src/blocksBase/common/clearEncoderBlock.cpp \
	$$PWD/src/blocksBase/common/emptyBlock.cpp \
	$$PWD/src/blocksBase/common/engineCommandBlock.cpp \
	$$PWD/src/blocksBase/common/enginesStopBlock.cpp \
	$$PWD/src/blocksBase/common/waitBlock.cpp \
	$$PWD/src/blocksBase/common/waitForAccelerometerBlock.cpp \
	$$PWD/src/blocksBase/common/waitForColorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForColorIntensityBlock.cpp \
	$$PWD/src/blocksBase/common/waitForColorSensorBlockBase.cpp \
	$$PWD/src/blocksBase/common/waitForEncoderBlock.cpp \
	$$PWD/src/blocksBase/common/waitForGyroscopeBlock.cpp \
	$$PWD/src/blocksBase/common/waitForLightSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForSonarDistanceBlock.cpp \
	$$PWD/src/blocksBase/common/waitForSoundSensorBlock.cpp \
	$$PWD/src/blocksBase/common/waitForTouchSensorBlock.cpp \
	$$PWD/src/robotModel/configuration.cpp \
	$$PWD/src/robotModel/commonRobotModel.cpp \
	$$PWD/src/robotModel/robotModelUtils.cpp \
	$$PWD/src/robotModel/portInfo.cpp \
	$$PWD/src/robotModel/deviceInfo.cpp \
	$$PWD/src/robotModel/robotParts/speaker.cpp \
	$$PWD/src/robotModel/robotParts/motor.cpp \
	$$PWD/src/robotModel/robotParts/display.cpp \
	$$PWD/src/robotModel/robotParts/buttons.cpp \
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

HEADERS += \
	$$PWD/include/interpreterBase/interpreterBaseDeclSpec.h \
	$$PWD/include/interpreterBase/kitPluginInterface.h \
	$$PWD/include/interpreterBase/additionalPreferences.h \
	$$PWD/include/interpreterBase/devicesConfigurationProvider.h \
	$$PWD/include/interpreterBase/eventsForKitPluginInterface.h \
	$$PWD/include/interpreterBase/interpreterControlInterface.h \
	$$PWD/include/interpreterBase/blocksBase/block.h \
	$$PWD/include/interpreterBase/blocksBase/blockInterface.h \
	$$PWD/include/interpreterBase/blocksBase/blockParserInterface.h \
	$$PWD/include/interpreterBase/blocksBase/blocksTableInterface.h \
	$$PWD/include/interpreterBase/blocksBase/blocksFactoryInterface.h \
	$$PWD/include/interpreterBase/blocksBase/commonBlocksFactory.h \
	$$PWD/include/interpreterBase/blocksBase/common/clearEncoderBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/emptyBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/engineCommandBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/enginesStopBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForAccelerometerBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForColorBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForColorIntensityBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForColorSensorBlockBase.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForEncoderBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForGyroscopeBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForLightSensorBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForSensorBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForSoundSensorBlock.h \
	$$PWD/include/interpreterBase/blocksBase/common/waitForTouchSensorBlock.h \
	$$PWD/include/interpreterBase/robotModel/configuration.h \
	$$PWD/include/interpreterBase/robotModel/commonRobotModel.h \
	$$PWD/include/interpreterBase/robotModel/configurationInterface.h \
	$$PWD/include/interpreterBase/robotModel/portInfo.h \
	$$PWD/include/interpreterBase/robotModel/deviceInfo.h \
	$$PWD/include/interpreterBase/robotModel/direction.h \
	$$PWD/include/interpreterBase/robotModel/robotModelManagerInterface.h \
	$$PWD/include/interpreterBase/robotModel/robotModelInterface.h \
	$$PWD/include/interpreterBase/robotModel/robotModelUtils.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/speaker.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/motor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/display.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/buttons.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/abstractSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/accelerometerSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensorPassive.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensorFull.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensorRed.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensorGreen.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensorBlue.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/encoderSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/gyroscopeSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/lightSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/device.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/rangeSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/scalarSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/soundSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/touchSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/vectorSensor.h \

# qrgui code
HEADERS += \
	$$PWD/../../../../qrgui/dialogs/preferencesPages/preferencesPage.h \

RESOURCES += \
	interpreterBase.qrc \
