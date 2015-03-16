QT += widgets

includes(plugins/robots/common/trikKit \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base qextserialport qslog \
)

HEADERS += \
	$$PWD/include/trikKit/robotModel/trikRobotModelBase.h \
	$$PWD/include/trikKit/robotModel/parts/trikColorSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikDisplay.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadButton.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadConnectionIndicator.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadPad.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadPadPressSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikGamepadWheel.h \
	$$PWD/include/trikKit/robotModel/parts/trikSpeaker.h \
	$$PWD/include/trikKit/robotModel/parts/trikPowerMotor.h \
	$$PWD/include/trikKit/robotModel/parts/trikServoMotor.h \
	$$PWD/include/trikKit/robotModel/parts/trikInfraredSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikLed.h \
	$$PWD/include/trikKit/robotModel/parts/trikLineSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikMotionSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikSonarSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikObjectSensor.h \
	$$PWD/include/trikKit/robotModel/parts/trikShell.h \
	$$PWD/include/trikKit/blocks/trikBlocksFactory.h \
	$$PWD/src/blocks/details/setBackgroundBlock.h \
	$$PWD/src/blocks/details/smileBlock.h \
	$$PWD/src/blocks/details/trikEnginesForwardBlock.h \
	$$PWD/src/blocks/details/trikEnginesBackwardBlock.h \
	$$PWD/src/blocks/details/waitForMotionBlock.h \
	$$PWD/src/blocks/details/speakerBlock.h \
	$$PWD/src/blocks/details/ledBlock.h \
	$$PWD/src/blocks/details/sayBlock.h \
	$$PWD/src/blocks/details/systemCommandBlock.h \
	$$PWD/src/blocks/details/waitGamepadButtonBlock.h \
	$$PWD/src/blocks/details/waitGamepadConnectBlock.h \
	$$PWD/src/blocks/details/waitGamepadDisconnectBlock.h \
	$$PWD/src/blocks/details/waitGamepadWheelBlock.h \
	$$PWD/src/blocks/details/waitPadPressBlock.h \
	$$PWD/src/blocks/details/drawLineBlock.h \
	$$PWD/src/blocks/details/drawPixelBlock.h \
	$$PWD/src/blocks/details/drawRectBlock.h \
	$$PWD/src/blocks/details/drawEllipseBlock.h \
	$$PWD/src/blocks/details/drawArcBlock.h \
	$$PWD/src/blocks/details/setPainterWidthBlock.h \
	$$PWD/src/blocks/details/setPainterColorBlock.h \

SOURCES += \
	$$PWD/src/robotModel/trikRobotModelBase.cpp \
	$$PWD/src/robotModel/parts/trikColorSensor.cpp \
	$$PWD/src/robotModel/parts/trikDisplay.cpp \
	$$PWD/src/robotModel/parts/trikGamepadButton.cpp \
	$$PWD/src/robotModel/parts/trikGamepadConnectionIndicator.cpp \
	$$PWD/src/robotModel/parts/trikGamepadPad.cpp \
	$$PWD/src/robotModel/parts/trikGamepadPadPressSensor.cpp \
	$$PWD/src/robotModel/parts/trikGamepadWheel.cpp \
	$$PWD/src/robotModel/parts/trikInfraredSensor.cpp \
	$$PWD/src/robotModel/parts/trikLed.cpp \
	$$PWD/src/robotModel/parts/trikLineSensor.cpp \
	$$PWD/src/robotModel/parts/trikMotionSensor.cpp \
	$$PWD/src/robotModel/parts/trikObjectSensor.cpp \
	$$PWD/src/robotModel/parts/trikPowerMotor.cpp \
	$$PWD/src/robotModel/parts/trikServoMotor.cpp \
	$$PWD/src/robotModel/parts/trikSonarSensor.cpp \
	$$PWD/src/robotModel/parts/trikSpeaker.cpp \
	$$PWD/src/robotModel/parts/trikShell.cpp \
	$$PWD/src/blocks/trikBlocksFactory.cpp \
	$$PWD/src/blocks/details/setBackgroundBlock.cpp \
	$$PWD/src/blocks/details/smileBlock.cpp \
	$$PWD/src/blocks/details/trikEnginesForwardBlock.cpp \
	$$PWD/src/blocks/details/trikEnginesBackwardBlock.cpp \
	$$PWD/src/blocks/details/waitForMotionBlock.cpp \
	$$PWD/src/blocks/details/speakerBlock.cpp \
	$$PWD/src/blocks/details/ledBlock.cpp \
	$$PWD/src/blocks/details/sayBlock.cpp \
	$$PWD/src/blocks/details/systemCommandBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadButtonBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadConnectBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadDisconnectBlock.cpp \
	$$PWD/src/blocks/details/waitGamepadWheelBlock.cpp \
	$$PWD/src/blocks/details/waitPadPressBlock.cpp \
	$$PWD/src/blocks/details/drawLineBlock.cpp \
	$$PWD/src/blocks/details/drawPixelBlock.cpp \
	$$PWD/src/blocks/details/drawRectBlock.cpp \
	$$PWD/src/blocks/details/drawEllipseBlock.cpp \
	$$PWD/src/blocks/details/drawArcBlock.cpp \
	$$PWD/src/blocks/details/setPainterWidthBlock.cpp \
	$$PWD/src/blocks/details/setPainterColorBlock.cpp \

TRANSLATIONS += $$PWD/../../../../qrtranslations/ru/plugins/robots/trikKit_ru.ts
