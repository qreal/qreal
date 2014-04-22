QT += widgets

INCLUDEPATH += \
	$$PWD \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../commonTwoDModel/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqrutils -lutils -linterpreterBase -lcommonTwoDModel -lqextserialport

HEADERS += \
	$$PWD/trikKitInterpreterPlugin.h \
	$$PWD/trikAdditionalPreferences.h \
	$$PWD/trikTwoDModelConfigurer.h \
	$$PWD/blocks/trikBlocksFactory.h \
	$$PWD/robotModel/trikRobotModelBase.h \
	$$PWD/robotModel/parts/trikDisplay.h \
	$$PWD/robotModel/parts/trikSpeaker.h \
	$$PWD/robotModel/parts/trikButtons.h \
	$$PWD/robotModel/parts/trikPowerMotor.h \
	$$PWD/robotModel/parts/trikServoMotor.h \
	$$PWD/robotModel/parts/trikInfraredSensor.h \
	$$PWD/robotModel/parts/trikSonarSensor.h \
	$$PWD/robotModel/parts/trikLed.h \
	$$PWD/robotModel/parts/trikCameraLineDetector.h \
	$$PWD/robotModel/parts/trikMovementSensor.h \
	$$PWD/robotModel/real/realRobotModelV4.h \
	$$PWD/robotModel/real/realRobotModelV5.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/buttons.h \
	$$PWD/robotModel/real/parts/powerMotor.h \
	$$PWD/robotModel/real/parts/servoMotor.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/infraredSensor.h \
	$$PWD/robotModel/real/parts/sonarSensor.h \
	$$PWD/robotModel/real/parts/led.h \
	$$PWD/robotModel/real/parts/cameraLineDetector.h \
	$$PWD/robotModel/real/parts/movementSensor.h \
#	$$PWD/robotModel/twoD/twoDRobotModel.h \
#	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
#	$$PWD/robotModel/twoD/parts/twoDButtons.h \

SOURCES += \
	$$PWD/trikKitInterpreterPlugin.cpp \
	$$PWD/trikAdditionalPreferences.cpp \
	$$PWD/trikTwoDModelConfigurer.cpp \
	$$PWD/blocks/trikBlocksFactory.cpp \
	$$PWD/robotModel/trikRobotModelBase.cpp \
	$$PWD/robotModel/parts/trikDisplay.cpp \
	$$PWD/robotModel/parts/trikSpeaker.cpp \
	$$PWD/robotModel/parts/trikButtons.cpp \
	$$PWD/robotModel/parts/trikPowerMotor.cpp \
	$$PWD/robotModel/parts/trikServoMotor.cpp \
	$$PWD/robotModel/parts/trikInfraredSensor.cpp \
	$$PWD/robotModel/parts/trikSonarSensor.cpp \
	$$PWD/robotModel/parts/trikLed.cpp \
	$$PWD/robotModel/parts/trikCameraLineDetector.cpp \
	$$PWD/robotModel/parts/trikMovementSensor.cpp \
	$$PWD/robotModel/real/realRobotModelV4.cpp \
	$$PWD/robotModel/real/realRobotModelV5.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/buttons.cpp \
	$$PWD/robotModel/real/parts/powerMotor.cpp \
	$$PWD/robotModel/real/parts/servoMotor.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/infraredSensor.cpp \
	$$PWD/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/robotModel/real/parts/led.cpp \
	$$PWD/robotModel/real/parts/cameraLineDetector.cpp \
	$$PWD/robotModel/real/parts/movementSensor.cpp \
#	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
#	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
#	$$PWD/robotModel/twoD/parts/twoDButtons.cpp \

FORMS += \
	$$PWD/trikAdditionalPreferences.ui \

RESOURCES += \
	$$PWD/../trikKitInterpreter.qrc \

include($$PWD/../qrguiIncludes.pri)
