TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../qrgui/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS += \
	robotsPlugin.h \

SOURCES += \
	robotsPlugin.cpp \

HEADERS += \
	interpreter.h \
	sensorConstants.h \
	robotCommunicationInterface.h \
	bluetoothRobotCommunication.h \
	details/thread.h \
	details/blocksFactory.h \
	details/blocksTable.h \
	details/bluetoothRobotCommunicationThread.h \
	details/robotCommandConstants.h \
	details/robotsBlockParser.h \
	details/autoconfigurer.h \
	details/blocks/block.h \
	details/blocks/dummyBlock.h \
	details/blocks/timerBlock.h \
	details/blocks/beepBlock.h \
	details/blocks/playToneBlock.h \
	details/blocks/initialBlock.h \
	details/blocks/finalBlock.h \
	details/blocks/waitForTouchSensorBlock.h \
	details/blocks/waitForSonarDistanceBlock.h \
	details/blocks/engineCommandBlock.h \
	details/blocks/enginesForwardBlock.h \
	details/blocks/enginesBackwardBlock.h \
	details/blocks/enginesStopBlock.h \
	details/blocks/loopBlock.h \
	details/blocks/forkBlock.h \
	details/blocks/waitForColorBlock.h \
	details/blocks/waitForColorIntensityBlock.h \
	details/blocks/functionBlock.h \
	details/blocks/ifBlock.h \
	details/robotParts/robotModel.h \
	details/robotParts/brick.h \
	details/robotParts/motor.h \
	details/robotParts/sensor.h \
	details/robotParts/touchSensor.h \
	details/robotParts/sonarSensor.h \
	details/robotParts/colorSensor.h \
	details/robotImplementations/sensorImplementations/abstractSensorImplementation.h \
	details/robotImplementations/sensorImplementations/bluetoothSensorImplementation.h \
	details/robotImplementations/sensorImplementations/bluetoothTouchSensorImplementation.h \
	details/robotImplementations/sensorImplementations/bluetoothSonarSensorImplementation.h \
	details/robotImplementations/sensorImplementations/bluetoothColorSensorImplementation.h \
	details/robotImplementations/sensorImplementations/nullSensorImplementation.h \
	details/robotImplementations/sensorImplementations/nullTouchSensorImplementation.h \
	details/robotImplementations/sensorImplementations/nullSonarSensorImplementation.h \
	details/robotImplementations/sensorImplementations/nullColorSensorImplementation.h \
	details/robotImplementations/sensorImplementations/unrealSensorImplementation.h \
	details/robotImplementations/sensorImplementations/unrealTouchSensorImplementation.h \
	details/robotImplementations/sensorImplementations/unrealSonarSensorImplementation.h \
	details/robotImplementations/sensorImplementations/unrealColorSensorImplementation.h \
	details/robotImplementations/motorImplementations/abstractMotorImplementation.h \
	details/robotImplementations/motorImplementations/realMotorImplementation.h \
	details/robotImplementations/motorImplementations/nullMotorImplementation.h \
	details/robotImplementations/motorImplementations/unrealMotorImplementation.h \
	details/robotImplementations/brickImplementations/abstractBrickImplementation.h \
	details/robotImplementations/brickImplementations/realBrickImplementation.h \
	details/robotImplementations/brickImplementations/nullBrickImplementation.h \
	details/robotImplementations/brickImplementations/unrealBrickImplementation.h \
	details/robotImplementations/abstractRobotModelImplementation.h \
	details/robotImplementations/realRobotModelImplementation.h \
	details/robotImplementations/nullRobotModelImplementation.h \
	details/robotImplementations/unrealRobotModelImplementation.h \
	details/d2RobotModel/d2ModelWidget.h \
	details/d2RobotModel/d2ModelScene.h \
	details/d2RobotModel/d2RobotModel.h \
	details/d2RobotModel/robotItem.h \
	details/d2RobotModel/sensorItem.h \
	details/d2RobotModel/rotater.h \
	details/d2RobotModel/sonarSensorItem.h \
	details/d2RobotModel/robotModelInterface.h \
	details/d2RobotModel/sensorsConfiguration.h \
	details/d2RobotModel/worldModel.h \

SOURCES += \
	interpreter.cpp \
	bluetoothRobotCommunication.cpp \
	details/thread.cpp \
	details/blocksTable.cpp \
	details/blocksFactory.cpp \
	details/bluetoothRobotCommunicationThread.cpp \
	details/robotsBlockParser.cpp \
	details/autoconfigurer.cpp \
	details/blocks/block.cpp \
	details/blocks/dummyBlock.cpp \
	details/blocks/timerBlock.cpp \
	details/blocks/beepBlock.cpp \
	details/blocks/playToneBlock.cpp \
	details/blocks/initialBlock.cpp \
	details/blocks/finalBlock.cpp \
	details/blocks/waitForTouchSensorBlock.cpp \
	details/blocks/waitForSonarDistanceBlock.cpp \
	details/blocks/engineCommandBlock.cpp \
	details/blocks/enginesForwardBlock.cpp \
	details/blocks/enginesBackwardBlock.cpp \
	details/blocks/enginesStopBlock.cpp \
	details/blocks/loopBlock.cpp \
	details/blocks/forkBlock.cpp \
	details/blocks/waitForColorBlock.cpp \
	details/blocks/waitForColorIntensityBlock.cpp \
	details/blocks/functionBlock.cpp \
	details/blocks/ifBlock.cpp \
	details/robotParts/robotModel.cpp \
	details/robotParts/touchSensor.cpp \
	details/robotParts/sonarSensor.cpp \
	details/robotParts/colorSensor.cpp \
	details/robotParts/sensor.cpp \
	details/robotParts/motor.cpp \
	details/robotParts/brick.cpp \
	details/robotImplementations/sensorImplementations/abstractSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/bluetoothSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/bluetoothTouchSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/bluetoothSonarSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/bluetoothColorSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/nullSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/nullTouchSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/nullSonarSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/nullColorSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/unrealSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/unrealTouchSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/unrealSonarSensorImplementation.cpp \
	details/robotImplementations/sensorImplementations/unrealColorSensorImplementation.cpp \
	details/robotImplementations/motorImplementations/abstractMotorImplementation.cpp \
	details/robotImplementations/motorImplementations/realMotorImplementation.cpp \
	details/robotImplementations/motorImplementations/nullMotorImplementation.cpp \
	details/robotImplementations/motorImplementations/unrealMotorImplementation.cpp \
	details/robotImplementations/brickImplementations/abstractBrickImplementation.cpp \
	details/robotImplementations/brickImplementations/realBrickImplementation.cpp \
	details/robotImplementations/brickImplementations/nullBrickImplementation.cpp \
	details/robotImplementations/brickImplementations/unrealBrickImplementation.cpp \
	details/robotImplementations/abstractRobotModelImplementation.cpp \
	details/robotImplementations/realRobotModelImplementation.cpp \
	details/robotImplementations/nullRobotModelImplementation.cpp \
	details/robotImplementations/unrealRobotModelImplementation.cpp \
	details/d2RobotModel/d2ModelWidget.cpp \
	details/d2RobotModel/d2ModelScene.cpp \
	details/d2RobotModel/d2RobotModel.cpp \
	details/d2RobotModel/robotItem.cpp \
	details/d2RobotModel/sensorItem.cpp \
	details/d2RobotModel/rotater.cpp \
	details/d2RobotModel/sonarSensorItem.cpp \
	details/d2RobotModel/sensorsConfiguration.cpp \
	details/d2RobotModel/worldModel.cpp \

FORMS += \
	details/d2RobotModel/d2Form.ui \


HEADERS += \
	../../../qrgui/kernel/ids.h \

SOURCES += \
	../../../qrgui/kernel/ids.cpp \
