QT += widgets

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model qextserialport \
		qslog \
		)

includes(plugins/robots/common/ev3Kit \
		plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/utils \
		qrtext \
)

HEADERS += \
	$$PWD/include/ev3Kit/robotModel/ev3RobotModelBase.h \
	$$PWD/include/ev3Kit/robotModel/parts/ev3Display.h \
	$$PWD/include/ev3Kit/robotModel/parts/ev3Speaker.h \
	$$PWD/include/ev3Kit/robotModel/parts/ev3Motor.h \
	$$PWD/include/ev3Kit/blocks/ev3BlocksFactory.h \
	$$PWD/src/blocks/details/speakerBlock.h \
	$$PWD/src/blocks/details/beepBlock.h \
	$$PWD/src/blocks/details/playToneBlock.h \
	$$PWD/src/blocks/details/drawCircleBlock.h \
	$$PWD/src/blocks/details/drawLineBlock.h \
	$$PWD/src/blocks/details/drawPixelBlock.h \
	$$PWD/src/blocks/details/drawRectBlock.h \
	$$PWD/src/blocks/details/ev3EnginesBackwardBlock.h \
	$$PWD/src/blocks/details/ev3EnginesForwardBlock.h \

SOURCES += \
	$$PWD/src/robotModel/ev3RobotModelBase.cpp \
	$$PWD/src/robotModel/parts/ev3Display.cpp \
	$$PWD/src/robotModel/parts/ev3Speaker.cpp \
	$$PWD/src/robotModel/parts/ev3Motor.cpp \
	$$PWD/src/blocks/ev3BlocksFactory.cpp \
	$$PWD/src/blocks/details/speakerBlock.cpp \
	$$PWD/src/blocks/details/beepBlock.cpp \
	$$PWD/src/blocks/details/playToneBlock.cpp \
	$$PWD/src/blocks/details/drawCircleBlock.cpp \
	$$PWD/src/blocks/details/drawLineBlock.cpp \
	$$PWD/src/blocks/details/drawPixelBlock.cpp \
	$$PWD/src/blocks/details/drawRectBlock.cpp \
	$$PWD/src/blocks/details/ev3EnginesBackwardBlock.cpp \
	$$PWD/src/blocks/details/ev3EnginesForwardBlock.cpp \

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/ev3Kit_ru.ts
