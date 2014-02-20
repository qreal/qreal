QT += widgets

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrutils

DEFINES += ROBOTS_INTERPRETER_BASE_LIBRARY

SOURCES += \
	$$PWD/src/sensorsConfigurationProvider.cpp \
	$$PWD/src/blocksBase/block.cpp \
	$$PWD/src/robotModel/configuration.cpp \
	$$PWD/src/robotModel/commonRobotModel.cpp \
	$$PWD/src/robotModel/portInfo.cpp \
	$$PWD/src/robotModel/deviceInfo.cpp \
	$$PWD/src/robotModel/robotParts/speaker.cpp \
	$$PWD/src/robotModel/robotParts/motor.cpp \
	$$PWD/src/robotModel/robotParts/display.cpp \
	$$PWD/src/robotModel/robotParts/abstractSensor.cpp \
	$$PWD/src/robotModel/robotParts/accelerometerSensor.cpp \
	$$PWD/src/robotModel/robotParts/colorSensor.cpp \
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
	$$PWD/include/interpreterBase/sensorsConfigurationProvider.h \
	$$PWD/include/interpreterBase/blocksBase/block.h \
	$$PWD/include/interpreterBase/blocksBase/blockInterface.h \
	$$PWD/include/interpreterBase/blocksBase/blockParserInterface.h \
	$$PWD/include/interpreterBase/blocksBase/blocksTableInterface.h \
	$$PWD/include/interpreterBase/blocksBase/blocksFactoryInterface.h \
	$$PWD/include/interpreterBase/robotModel/configuration.h \
	$$PWD/include/interpreterBase/robotModel/commonRobotModel.h \
	$$PWD/include/interpreterBase/robotModel/configurationInterface.h \
	$$PWD/include/interpreterBase/robotModel/portInfo.h \
	$$PWD/include/interpreterBase/robotModel/deviceInfo.h \
	$$PWD/include/interpreterBase/robotModel/robotModelManagerInterface.h \
	$$PWD/include/interpreterBase/robotModel/robotModelInterface.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/speaker.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/motor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/display.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/abstractSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/accelerometerSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensor.h \
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
