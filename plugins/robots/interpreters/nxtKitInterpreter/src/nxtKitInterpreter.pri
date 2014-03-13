QT += widgets

INCLUDEPATH += \
	$$PWD \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../commonTwoDModel/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lutils -linterpreterBase -lcommonTwoDModel -lqextserialport

HEADERS += \
	$$PWD/nxtKitInterpreterPlugin.h \
	$$PWD/nxtAdditionalPreferences.h \
	$$PWD/robotModel/nxtRobotModelBase.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/parts/nxtInputDevice.h \
	$$PWD/robotModel/real/parts/display.h \
	$$PWD/robotModel/real/parts/speaker.h \
	$$PWD/robotModel/real/parts/motor.h \
	$$PWD/robotModel/real/parts/touchSensor.h \
	$$PWD/robotModel/real/parts/colorSensor.h \
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
	$$PWD/robotModel/twoD/twoDRobotModel.h \

SOURCES += \
	$$PWD/nxtKitInterpreterPlugin.cpp \
	$$PWD/nxtAdditionalPreferences.cpp \
	$$PWD/robotModel/nxtRobotModelBase.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/parts/nxtInputDevice.cpp \
	$$PWD/robotModel/real/parts/display.cpp \
	$$PWD/robotModel/real/parts/speaker.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
	$$PWD/robotModel/real/parts/touchSensor.cpp \
	$$PWD/robotModel/real/parts/colorSensor.cpp \
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
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \

FORMS += \
	$$PWD/nxtAdditionalPreferences.ui \

include($$PWD/../qrguiIncludes.pri)
include($$PWD/communication/communication.pri)
