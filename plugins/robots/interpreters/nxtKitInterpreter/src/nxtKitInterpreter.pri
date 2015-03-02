QT += widgets

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-interpreter-base robots-common-2d-model qextserialport \
		qslog \
		)

includes(plugins/robots/interpreters/interpreterBase plugins/robots/interpreters/commonTwoDModel plugins/robots/utils qrtext plugins/robots/interpreters/nxtKitInterpreter/src) \

HEADERS += \
	$$PWD/commandConstants.h \
	$$PWD/nxtAdditionalPreferences.h \
	$$PWD/nxtDisplayWidget.h \
	$$PWD/nxtKitInterpreterPlugin.h \
	$$PWD/blocks/nxtBlocksFactory.h \
	$$PWD/blocks/details/speakerBlock.h \
	$$PWD/blocks/details/beepBlock.h \
	$$PWD/blocks/details/playToneBlock.h \
	$$PWD/blocks/details/drawCircleBlock.h \
	$$PWD/blocks/details/drawLineBlock.h \
	$$PWD/blocks/details/drawPixelBlock.h \
	$$PWD/blocks/details/drawRectBlock.h \
	$$PWD/blocks/details/nxtEnginesBackwardBlock.h \
	$$PWD/blocks/details/nxtEnginesForwardBlock.h \
	$$PWD/robotModel/nxtRobotModelBase.h \
	$$PWD/robotModel/parts/nxtDisplay.h \
	$$PWD/robotModel/parts/nxtSpeaker.h \
	$$PWD/robotModel/parts/nxtMotor.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/parts/nxtInputDevice.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/button.h \
	$$PWD/robotModel/real/parts/motor.h \
	$$PWD/robotModel/real/parts/touchSensor.h \
	$$PWD/robotModel/real/parts/colorSensorFull.h \
	$$PWD/robotModel/real/parts/colorSensorRed.h \
	$$PWD/robotModel/real/parts/colorSensorGreen.h \
	$$PWD/robotModel/real/parts/colorSensorBlue.h \
	$$PWD/robotModel/real/parts/colorSensorPassive.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/gyroscopeSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/real/parts/sonarSensor.h \
	$$PWD/robotModel/real/parts/soundSensor.h \
	$$PWD/robotModel/real/parts/implementations/colorSensorImpl.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.h \
	$$PWD/robotModel/twoD/parts/twoDMotor.h \

SOURCES += \
	$$PWD/nxtAdditionalPreferences.cpp \
	$$PWD/nxtDisplayWidget.cpp \
	$$PWD/nxtKitInterpreterPlugin.cpp \
	$$PWD/blocks/nxtBlocksFactory.cpp \
	$$PWD/blocks/details/speakerBlock.cpp \
	$$PWD/blocks/details/beepBlock.cpp \
	$$PWD/blocks/details/playToneBlock.cpp \
	$$PWD/blocks/details/drawCircleBlock.cpp \
	$$PWD/blocks/details/drawLineBlock.cpp \
	$$PWD/blocks/details/drawPixelBlock.cpp \
	$$PWD/blocks/details/drawRectBlock.cpp \
	$$PWD/blocks/details/nxtEnginesBackwardBlock.cpp \
	$$PWD/blocks/details/nxtEnginesForwardBlock.cpp \
	$$PWD/robotModel/nxtRobotModelBase.cpp \
	$$PWD/robotModel/parts/nxtDisplay.cpp \
	$$PWD/robotModel/parts/nxtSpeaker.cpp \
	$$PWD/robotModel/parts/nxtMotor.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/parts/nxtInputDevice.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/button.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
	$$PWD/robotModel/real/parts/touchSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensorFull.cpp \
	$$PWD/robotModel/real/parts/colorSensorRed.cpp \
	$$PWD/robotModel/real/parts/colorSensorGreen.cpp \
	$$PWD/robotModel/real/parts/colorSensorBlue.cpp \
	$$PWD/robotModel/real/parts/colorSensorPassive.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/gyroscopeSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/real/parts/sonarSensor.cpp \
	$$PWD/robotModel/real/parts/soundSensor.cpp \
	$$PWD/robotModel/real/parts/implementations/colorSensorImpl.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDSpeaker.cpp \
	$$PWD/robotModel/twoD/parts/twoDMotor.cpp \

FORMS += \
	$$PWD/nxtAdditionalPreferences.ui \
	$$PWD/nxtDisplayWidget.ui \

include($$PWD/communication/communication.pri)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtKitInterpreter_ru.ts

RESOURCES += $$PWD/../nxtKitInterpreter.qrc
