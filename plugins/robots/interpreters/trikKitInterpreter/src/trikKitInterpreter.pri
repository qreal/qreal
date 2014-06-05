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
	$$PWD/trikAdditionalPreferences.h \
	$$PWD/trikDisplayWidget.h \
	$$PWD/trikKitInterpreterPlugin.h \
	$$PWD/trikTwoDModelConfigurer.h \
	$$PWD/blocks/trikBlocksFactory.h \
	$$PWD/blocks/details/sadSmileBlock.h \
	$$PWD/blocks/details/setBackgroundBlock.h \
	$$PWD/blocks/details/smileBlock.h \
	$$PWD/blocks/details/smileBlockBase.h \
	$$PWD/blocks/details/trikEnginesForwardBlock.h \
	$$PWD/blocks/details/trikEnginesBackwardBlock.h \
	$$PWD/robotModel/trikRobotModelBase.h \
	$$PWD/robotModel/parts/trikDisplay.h \
	$$PWD/robotModel/parts/trikSpeaker.h \
	$$PWD/robotModel/parts/trikPowerMotor.h \
	$$PWD/robotModel/parts/trikServoMotor.h \
	$$PWD/robotModel/parts/trikInfraredSensor.h \
	$$PWD/robotModel/parts/trikSonarSensor.h \
	$$PWD/robotModel/parts/trikLed.h \
	$$PWD/robotModel/parts/trikCameraLineDetector.h \
	$$PWD/robotModel/parts/trikMovementSensor.h \
	$$PWD/robotModel/real/realRobotModelBase.h \
	$$PWD/robotModel/real/realRobotModelV4.h \
	$$PWD/robotModel/real/realRobotModelV6.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/button.h \
	$$PWD/robotModel/real/parts/powerMotor.h \
	$$PWD/robotModel/real/parts/servoMotor.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/infraredSensor.h \
	$$PWD/robotModel/real/parts/sonarSensor.h \
	$$PWD/robotModel/real/parts/led.h \
	$$PWD/robotModel/real/parts/cameraLineDetector.h \
	$$PWD/robotModel/real/parts/movementSensor.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/robotModel/twoD/parts/twoDInfraredSensor.h \

SOURCES += \
	$$PWD/trikAdditionalPreferences.cpp \
	$$PWD/trikKitInterpreterPlugin.cpp \
	$$PWD/trikDisplayWidget.cpp \
	$$PWD/trikTwoDModelConfigurer.cpp \
	$$PWD/blocks/trikBlocksFactory.cpp \
	$$PWD/blocks/details/sadSmileBlock.cpp \
	$$PWD/blocks/details/setBackgroundBlock.cpp \
	$$PWD/blocks/details/smileBlock.cpp \
	$$PWD/blocks/details/smileBlockBase.cpp \
	$$PWD/blocks/details/trikEnginesForwardBlock.cpp \
	$$PWD/blocks/details/trikEnginesBackwardBlock.cpp \
	$$PWD/robotModel/trikRobotModelBase.cpp \
	$$PWD/robotModel/parts/trikDisplay.cpp \
	$$PWD/robotModel/parts/trikSpeaker.cpp \
	$$PWD/robotModel/parts/trikPowerMotor.cpp \
	$$PWD/robotModel/parts/trikServoMotor.cpp \
	$$PWD/robotModel/parts/trikInfraredSensor.cpp \
	$$PWD/robotModel/parts/trikSonarSensor.cpp \
	$$PWD/robotModel/parts/trikLed.cpp \
	$$PWD/robotModel/parts/trikCameraLineDetector.cpp \
	$$PWD/robotModel/parts/trikMovementSensor.cpp \
	$$PWD/robotModel/real/realRobotModelBase.cpp \
	$$PWD/robotModel/real/realRobotModelV4.cpp \
	$$PWD/robotModel/real/realRobotModelV6.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/button.cpp \
	$$PWD/robotModel/real/parts/powerMotor.cpp \
	$$PWD/robotModel/real/parts/servoMotor.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/infraredSensor.cpp \
	$$PWD/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/robotModel/real/parts/led.cpp \
	$$PWD/robotModel/real/parts/cameraLineDetector.cpp \
	$$PWD/robotModel/real/parts/movementSensor.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/robotModel/twoD/parts/twoDInfraredSensor.cpp \

FORMS += \
	$$PWD/trikAdditionalPreferences.ui \
	$$PWD/trikDisplayWidget.ui \

RESOURCES += \
	$$PWD/../trikKitInterpreter.qrc \

TRANSLATIONS += $$PWD/../trikKitInterpreter_ru.ts

include($$PWD/../qrguiIncludes.pri)
