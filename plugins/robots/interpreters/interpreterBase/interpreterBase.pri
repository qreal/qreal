QT += widgets

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel

DEFINES += ROBOTS_INTERPRETER_BASE_LIBRARY

HEADERS += \
	$$PWD/src/baseBlocks/blocksFactory.h \
	$$PWD/src/baseBlocks/details/finalBlock.h \
	$$PWD/src/baseBlocks/details/initialBlock.h \
	$$PWD/src/robotModel/details/defaultRobotModel.h \

SOURCES += \
	$$PWD/src/baseBlocks/block.cpp \
	$$PWD/src/baseBlocks/blocksFactory.cpp \
	$$PWD/src/baseBlocks/details/finalBlock.cpp \
	$$PWD/src/baseBlocks/details/initialBlock.cpp \
	$$PWD/src/robotModel/configuration.cpp \
	$$PWD/src/robotModel/commonRobotModel.cpp \
	$$PWD/src/robotModel/portInfo.cpp \
	$$PWD/src/robotModel/pluggableDeviceInfo.cpp \
	$$PWD/src/robotModel/proxyRobotModel.cpp \
	$$PWD/src/robotModel/details/defaultRobotModel.cpp \
	$$PWD/src/robotModel/robotParts/pluggableDevice.cpp \

HEADERS += \
	$$PWD/include/interpreterBase/interpreterBaseDeclSpec.h \
	$$PWD/include/interpreterBase/kitPluginInterface.h \
	$$PWD/include/interpreterBase/baseBlocks/block.h \
	$$PWD/include/interpreterBase/baseBlocks/blockInterface.h \
	$$PWD/include/interpreterBase/baseBlocks/blockParserInterface.h \
	$$PWD/include/interpreterBase/baseBlocks/blocksTableInterface.h \
	$$PWD/include/interpreterBase/baseBlocks/blocksFactoryInterface.h \
	$$PWD/include/interpreterBase/robotModel/configuration.h \
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
