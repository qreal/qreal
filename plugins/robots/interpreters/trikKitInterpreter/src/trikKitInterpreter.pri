QT += widgets network

INCLUDEPATH += \
	$$PWD \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../commonTwoDModel/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \
	$$PWD/../../../../../qrtext/include \

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqrutils -lqrtext \
		-lqrgui-preferences-dialog -lqrgui-tool-plugin-interface \
		-lrobots-utils -lrobots-interpreter-base -lrobots-common-2d-model -lqextserialport \

HEADERS += \
	$$PWD/trikAdditionalPreferences.h \
	$$PWD/trikDisplayWidget.h \
	$$PWD/trikKitInterpreterPlugin.h \
	$$PWD/blocks/trikBlocksFactory.h \
	$$PWD/blocks/details/setBackgroundBlock.h \
	$$PWD/blocks/details/smileBlock.h \
	$$PWD/blocks/details/trikEnginesForwardBlock.h \
	$$PWD/blocks/details/trikEnginesBackwardBlock.h \
	$$PWD/blocks/details/waitForMotionBlock.h \
	$$PWD/blocks/details/speakerBlock.h \
	$$PWD/blocks/details/ledBlock.h \
	$$PWD/blocks/details/sayBlock.h \
	$$PWD/blocks/details/systemCommandBlock.h \
	$$PWD/robotModel/trikRobotModelBase.h \
	$$PWD/robotModel/parts/trikColorSensor.h \
	$$PWD/robotModel/parts/trikDisplay.h \
	$$PWD/robotModel/parts/trikSpeaker.h \
	$$PWD/robotModel/parts/trikPowerMotor.h \
	$$PWD/robotModel/parts/trikServoMotor.h \
	$$PWD/robotModel/parts/trikInfraredSensor.h \
	$$PWD/robotModel/parts/trikLed.h \
	$$PWD/robotModel/parts/trikLineSensor.h \
	$$PWD/robotModel/parts/trikMotionSensor.h \
	$$PWD/robotModel/parts/trikSonarSensor.h \
	$$PWD/robotModel/parts/trikObjectSensor.h \
	$$PWD/robotModel/parts/trikShell.h \
	$$PWD/robotModel/real/realRobotModelBase.h \
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
	$$PWD/robotModel/real/parts/lineSensor.h \
	$$PWD/robotModel/real/parts/motionSensor.h \
	$$PWD/robotModel/real/parts/objectSensor.h \
	$$PWD/robotModel/real/parts/colorSensor.h \
	$$PWD/robotModel/real/parts/gyroscope.h \
	$$PWD/robotModel/real/parts/accelerometer.h \
	$$PWD/robotModel/real/parts/shell.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/robotModel/twoD/parts/twoDInfraredSensor.h \
	$$PWD/robotModel/twoD/parts/twoDLineSensor.h \
	$$PWD/robotModel/twoD/parts/twoDObjectSensor.h \
	$$PWD/robotModel/twoD/parts/twoDColorSensor.h \
	$$PWD/blocks/details/drawLineBlock.h \
	$$PWD/blocks/details/drawPixelBlock.h \
	$$PWD/blocks/details/drawRectBlock.h \
	$$PWD/blocks/details/drawEllipseBlock.h \
	$$PWD/blocks/details/drawArcBlock.h \
	$$PWD/blocks/details/setPainterWidthBlock.h \
	$$PWD/blocks/details/setPainterColorBlock.h \
	$$PWD/robotModel/twoD/parts/twoDLed.h \
	$$PWD/trikFSharpAdditionalPreferences.h \
	$$PWD/trikWinScpAdditionalPreferences.h \

SOURCES += \
	$$PWD/trikAdditionalPreferences.cpp \
	$$PWD/trikFSharpAdditionalPreferences.cpp \
	$$PWD/trikWinScpAdditionalPreferences.cpp \
	$$PWD/trikKitInterpreterPlugin.cpp \
	$$PWD/trikDisplayWidget.cpp \
	$$PWD/blocks/trikBlocksFactory.cpp \
	$$PWD/blocks/details/setBackgroundBlock.cpp \
	$$PWD/blocks/details/smileBlock.cpp \
	$$PWD/blocks/details/trikEnginesForwardBlock.cpp \
	$$PWD/blocks/details/trikEnginesBackwardBlock.cpp \
	$$PWD/blocks/details/waitForMotionBlock.cpp \
	$$PWD/blocks/details/speakerBlock.cpp \
	$$PWD/blocks/details/ledBlock.cpp \
	$$PWD/blocks/details/sayBlock.cpp \
	$$PWD/blocks/details/systemCommandBlock.cpp \
	$$PWD/robotModel/trikRobotModelBase.cpp \
	$$PWD/robotModel/parts/trikColorSensor.cpp \
	$$PWD/robotModel/parts/trikDisplay.cpp \
	$$PWD/robotModel/parts/trikInfraredSensor.cpp \
	$$PWD/robotModel/parts/trikLed.cpp \
	$$PWD/robotModel/parts/trikLineSensor.cpp \
	$$PWD/robotModel/parts/trikMotionSensor.cpp \
	$$PWD/robotModel/parts/trikObjectSensor.cpp \
	$$PWD/robotModel/parts/trikPowerMotor.cpp \
	$$PWD/robotModel/parts/trikServoMotor.cpp \
	$$PWD/robotModel/parts/trikSonarSensor.cpp \
	$$PWD/robotModel/parts/trikSpeaker.cpp \
	$$PWD/robotModel/parts/trikShell.cpp \
	$$PWD/robotModel/real/realRobotModelBase.cpp \
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
	$$PWD/robotModel/real/parts/lineSensor.cpp \
	$$PWD/robotModel/real/parts/objectSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensor.cpp \
	$$PWD/robotModel/real/parts/motionSensor.cpp \
	$$PWD/robotModel/real/parts/gyroscope.cpp \
	$$PWD/robotModel/real/parts/accelerometer.cpp \
	$$PWD/robotModel/real/parts/shell.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/robotModel/twoD/parts/twoDInfraredSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDLineSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDObjectSensor.cpp \
	$$PWD/robotModel/twoD/parts/twoDColorSensor.cpp \
	$$PWD/blocks/details/drawLineBlock.cpp \
	$$PWD/blocks/details/drawPixelBlock.cpp \
	$$PWD/blocks/details/drawRectBlock.cpp \
	$$PWD/blocks/details/drawEllipseBlock.cpp \
	$$PWD/blocks/details/drawArcBlock.cpp \
	$$PWD/blocks/details/setPainterWidthBlock.cpp \
	$$PWD/blocks/details/setPainterColorBlock.cpp \
	$$PWD/robotModel/twoD/parts/twoDLed.cpp \

FORMS += \
	$$PWD/trikAdditionalPreferences.ui \
	$$PWD/trikDisplayWidget.ui \
	$$PWD/trikFSharpAdditionalPreferences.ui \
	$$PWD/trikWinScpAdditionalPreferences.ui \

RESOURCES += \
	$$PWD/../trikKitInterpreter.qrc \

TRANSLATIONS += $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikKitInterpreter_ru.ts
