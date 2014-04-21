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
	$$PWD/nxtKitInterpreterPlugin.h \
	$$PWD/nxtAdditionalPreferences.h \
	$$PWD/blocks/nxtBlocksFactory.h \
	$$PWD/blocks/details/speakerBlock.h \
	$$PWD/blocks/details/beepBlock.h \
	$$PWD/blocks/details/playToneBlock.h \
	$$PWD/blocks/details/displayBlock.h \
	$$PWD/blocks/details/clearScreenBlock.h \
	$$PWD/blocks/details/drawCircleBlock.h \
	$$PWD/blocks/details/drawLineBlock.h \
	$$PWD/blocks/details/drawPixelBlock.h \
	$$PWD/blocks/details/drawRectBlock.h \
	$$PWD/blocks/details/printTextBlock.h \
	$$PWD/blocks/details/waitForButtonsBlock.h \
	$$PWD/robotModel/nxtRobotModelBase.h \
	$$PWD/robotModel/parts/nxtDisplay.h \
	$$PWD/robotModel/parts/nxtSpeaker.h \
	$$PWD/robotModel/parts/nxtButtons.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/parts/nxtInputDevice.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/buttons.h \
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

SOURCES += \
	$$PWD/nxtKitInterpreterPlugin.cpp \
	$$PWD/nxtAdditionalPreferences.cpp \
	$$PWD/blocks/nxtBlocksFactory.cpp \
	$$PWD/blocks/details/speakerBlock.cpp \
	$$PWD/blocks/details/beepBlock.cpp \
	$$PWD/blocks/details/playToneBlock.cpp \
	$$PWD/blocks/details/displayBlock.cpp \
	$$PWD/blocks/details/clearScreenBlock.cpp \
	$$PWD/blocks/details/drawCircleBlock.cpp \
	$$PWD/blocks/details/drawLineBlock.cpp \
	$$PWD/blocks/details/drawPixelBlock.cpp \
	$$PWD/blocks/details/drawRectBlock.cpp \
	$$PWD/blocks/details/printTextBlock.cpp \
	$$PWD/blocks/details/waitForButtonsBlock.cpp \
	$$PWD/robotModel/nxtRobotModelBase.cpp \
	$$PWD/robotModel/parts/nxtDisplay.cpp \
	$$PWD/robotModel/parts/nxtSpeaker.cpp \
	$$PWD/robotModel/parts/nxtButtons.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/parts/nxtInputDevice.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/buttons.cpp \
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

FORMS += \
	$$PWD/nxtAdditionalPreferences.ui \

include($$PWD/../qrguiIncludes.pri)
include($$PWD/communication/communication.pri)
