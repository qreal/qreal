QT += widgets

INCLUDEPATH += \
	$$PWD \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../twoDModel/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lutils -linterpreterBase -ltwoDModel -lqextserialport

HEADERS += \
	$$PWD/nxtKitInterpreterPlugin.h \
	$$PWD/nxtAdditionalPreferences.h \
	$$PWD/robotModel/nxtRobotModelBase.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/real/parts/motor.h \
#	$$PWD/robotModel/real/parts/touchSensor.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \

SOURCES += \
	$$PWD/nxtKitInterpreterPlugin.cpp \
	$$PWD/nxtAdditionalPreferences.cpp \
	$$PWD/robotModel/nxtRobotModelBase.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/real/parts/motor.cpp \
#	$$PWD/robotModel/real/parts/touchSensor.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \

FORMS += \
	$$PWD/nxtAdditionalPreferences.ui \

include($$PWD/../qrguiIncludes.pri)
include($$PWD/communication/communication.pri)
