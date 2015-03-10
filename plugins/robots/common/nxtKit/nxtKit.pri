QT += widgets

includes(plugins/robots/common/nxtKit \
		plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base qextserialport qslog \
)

HEADERS += \
	$$PWD/include/nxtKit/robotModel/nxtRobotModelBase.h \
	$$PWD/include/nxtKit/robotModel/parts/nxtDisplay.h \
	$$PWD/include/nxtKit/robotModel/parts/nxtSpeaker.h \
	$$PWD/include/nxtKit/robotModel/parts/nxtMotor.h \
	$$PWD/include/nxtKit/blocks/nxtBlocksFactory.h \
	$$PWD/src/blocks/details/speakerBlock.h \
	$$PWD/src/blocks/details/beepBlock.h \
	$$PWD/src/blocks/details/playToneBlock.h \
	$$PWD/src/blocks/details/drawCircleBlock.h \
	$$PWD/src/blocks/details/drawLineBlock.h \
	$$PWD/src/blocks/details/drawPixelBlock.h \
	$$PWD/src/blocks/details/drawRectBlock.h \
	$$PWD/src/blocks/details/nxtEnginesBackwardBlock.h \
	$$PWD/src/blocks/details/nxtEnginesForwardBlock.h \

SOURCES += \
	$$PWD/src/robotModel/nxtRobotModelBase.cpp \
	$$PWD/src/robotModel/parts/nxtDisplay.cpp \
	$$PWD/src/robotModel/parts/nxtSpeaker.cpp \
	$$PWD/src/robotModel/parts/nxtMotor.cpp \
	$$PWD/src/blocks/nxtBlocksFactory.cpp \
	$$PWD/src/blocks/details/speakerBlock.cpp \
	$$PWD/src/blocks/details/beepBlock.cpp \
	$$PWD/src/blocks/details/playToneBlock.cpp \
	$$PWD/src/blocks/details/drawCircleBlock.cpp \
	$$PWD/src/blocks/details/drawLineBlock.cpp \
	$$PWD/src/blocks/details/drawPixelBlock.cpp \
	$$PWD/src/blocks/details/drawRectBlock.cpp \
	$$PWD/src/blocks/details/nxtEnginesBackwardBlock.cpp \
	$$PWD/src/blocks/details/nxtEnginesForwardBlock.cpp \

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/nxtKit_ru.ts
