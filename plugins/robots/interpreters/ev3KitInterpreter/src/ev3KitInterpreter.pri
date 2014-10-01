QT += widgets

INCLUDEPATH += \
        $$PWD \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \
	$$PWD/../../../../../qrtext/include \

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqrutils -lutils -linterpreterBase -lcommonTwoDModel -lqextserialport \
        -lqrtext

HEADERS += \
        $$PWD/commandConstants.h \
        $$PWD/ev3AdditionalPreferences.h \
	$$PWD/ev3KitInterpreterPlugin.h \
        $$PWD/robotModel/ev3RobotModelBase.h \
        $$PWD/robotModel/real/realRobotModel.h \
        $$PWD/robotModel/parts/ev3Display.h \
        $$PWD/robotModel/parts/ev3Speaker.h \
        $$PWD/robotModel/parts/ev3Motor.h \
        $$PWD/robotModel/real/parts/display.h \
        $$PWD/robotModel/real/parts/speaker.h \
        $$PWD/robotModel/real/parts/motor.h \
        $$PWD/blocks/ev3BlocksFactory.h \
        $$PWD/blocks/details/speakerBlock.h \
        $$PWD/blocks/details/beepBlock.h \
        $$PWD/blocks/details/playToneBlock.h \
        $$PWD/blocks/details/drawCircleBlock.h \
        $$PWD/blocks/details/drawLineBlock.h \
        $$PWD/blocks/details/drawPixelBlock.h \
        $$PWD/blocks/details/drawRectBlock.h \
        $$PWD/blocks/details/ev3EnginesBackwardBlock.h \
        $$PWD/blocks/details/ev3EnginesForwardBlock.h \


SOURCES += \
        $$PWD/ev3AdditionalPreferences.cpp \
	$$PWD/ev3KitInterpreterPlugin.cpp \
        $$PWD/robotModel/ev3RobotModelBase.cpp \
        $$PWD/robotModel/real/realRobotModel.cpp \
        $$PWD/robotModel/parts/ev3Display.cpp \
        $$PWD/robotModel/parts/ev3Speaker.cpp \
        $$PWD/robotModel/parts/ev3Motor.cpp \
        $$PWD/robotModel/real/parts/display.cpp \
        $$PWD/robotModel/real/parts/speaker.cpp \
        $$PWD/robotModel/real/parts/motor.cpp \
        $$PWD/blocks/ev3BlocksFactory.cpp \
        $$PWD/blocks/details/speakerBlock.cpp \
        $$PWD/blocks/details/beepBlock.cpp \
        $$PWD/blocks/details/playToneBlock.cpp \
        $$PWD/blocks/details/drawCircleBlock.cpp \
        $$PWD/blocks/details/drawLineBlock.cpp \
        $$PWD/blocks/details/drawPixelBlock.cpp \
        $$PWD/blocks/details/drawRectBlock.cpp \
        $$PWD/blocks/details/ev3EnginesBackwardBlock.cpp \
        $$PWD/blocks/details/ev3EnginesForwardBlock.cpp \






FORMS += \
        $$PWD/ev3AdditionalPreferences.ui \

include($$PWD/../qrguiIncludes.pri)
include($$PWD/communication/communication.pri)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3KitInterpreter_ru.ts
