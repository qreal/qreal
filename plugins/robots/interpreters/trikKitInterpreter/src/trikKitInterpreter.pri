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
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/buttons.h \
	$$PWD/robotModel/real/parts/motor.h \
	$$PWD/robotModel/real/parts/encoderSensor.h \
	$$PWD/robotModel/real/parts/lightSensor.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \
	$$PWD/robotModel/twoD/parts/twoDDisplay.h \
	$$PWD/robotModel/twoD/parts/twoDButtons.h \

SOURCES += \
	$$PWD/trikKitInterpreterPlugin.cpp \
	$$PWD/trikAdditionalPreferences.cpp \
	$$PWD/trikTwoDModelConfigurer.cpp \
	$$PWD/blocks/trikBlocksFactory.cpp \
	$$PWD/robotModel/trikRobotModelBase.cpp \
	$$PWD/robotModel/parts/trikDisplay.cpp \
	$$PWD/robotModel/parts/trikSpeaker.cpp \
	$$PWD/robotModel/parts/trikButtons.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/buttons.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
	$$PWD/robotModel/real/parts/encoderSensor.cpp \
	$$PWD/robotModel/real/parts/lightSensor.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \
	$$PWD/robotModel/twoD/parts/twoDDisplay.cpp \
	$$PWD/robotModel/twoD/parts/twoDButtons.cpp \

FORMS += \
	$$PWD/trikAdditionalPreferences.ui \

RESOURCES += \
	$$PWD/../trikKitInterpreter.qrc \

include($$PWD/../qrguiIncludes.pri)
