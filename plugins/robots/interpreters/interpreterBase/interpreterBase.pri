QT += widgets

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel

DEFINES += ROBOTS_INTERPRETER_BASE_LIBRARY

HEADERS += \
	$$PWD/src/blocks/blocksFactory.h \
	$$PWD/src/blocks/details/finalBlock.h \
	$$PWD/src/blocks/details/initialBlock.h \
	$$PWD/src/robotModel/details/defaultRobotModel.h \
	$$PWD/src/robotModel/details/defaultConfiguration.h \

SOURCES += \
	$$PWD/src/blocks/block.cpp \
	$$PWD/src/blocks/blocksFactory.cpp \
	$$PWD/src/blocks/details/finalBlock.cpp \
	$$PWD/src/blocks/details/initialBlock.cpp \
	$$PWD/src/robotModel/abstractConfiguration.cpp \
	$$PWD/src/robotModel/commonRobotModel.cpp \
	$$PWD/src/robotModel/portInfo.cpp \
	$$PWD/src/robotModel/pluggableDeviceInfo.cpp \
	$$PWD/src/robotModel/proxyRobotModel.cpp \
	$$PWD/src/robotModel/details/defaultRobotModel.cpp \
	$$PWD/src/robotModel/details/defaultConfiguration.cpp \

HEADERS += \
	$$PWD/include/interpreterBase/interpreterBaseDeclSpec.h \
	$$PWD/include/interpreterBase/kitPluginInterface.h \
	$$PWD/include/interpreterBase/blocks/block.h \
	$$PWD/include/interpreterBase/blocks/blockInterface.h \
	$$PWD/include/interpreterBase/blocks/blockParserInterface.h \
	$$PWD/include/interpreterBase/blocks/blocksTableInterface.h \
	$$PWD/include/interpreterBase/blocks/blocksFactoryInterface.h \
	$$PWD/include/interpreterBase/robotModel/abstractConfiguration.h \
	$$PWD/include/interpreterBase/robotModel/commonRobotModel.h \
	$$PWD/include/interpreterBase/robotModel/configurationInterface.h \
	$$PWD/include/interpreterBase/robotModel/portInfo.h \
	$$PWD/include/interpreterBase/robotModel/pluggableDeviceInfo.h \
	$$PWD/include/interpreterBase/robotModel/proxyRobotModel.h \
	$$PWD/include/interpreterBase/robotModel/robotModelInterface.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/brick.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/motor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/abstractSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/display.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/accelerometerSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/colorSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/encoderSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/gyroscopeSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/lightSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/pluggableDevice.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/rangeSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/scalarSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/soundSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/touchSensor.h \
	$$PWD/include/interpreterBase/robotModel/robotParts/vectorSensor.h \
