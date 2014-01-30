QT += widgets

INCLUDEPATH += \
	$$PWD \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../twoDModel/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lutils -linterpreterBase -ltwoDModel -lqextserialport

HEADERS += \
	$$PWD/nxtKitInterpreterPlugin.h \
	$$PWD/nxtAdditionalPreferences.h \
	$$PWD/robotModel/nxtRobotModelBase.h \
	$$PWD/robotModel/real/realRobotModel.h \
	$$PWD/robotModel/twoD/twoDRobotModel.h \

SOURCES += \
	$$PWD/nxtKitInterpreterPlugin.cpp \
	$$PWD/nxtAdditionalPreferences.cpp \
	$$PWD/robotModel/nxtRobotModelBase.cpp \
	$$PWD/robotModel/real/realRobotModel.cpp \
	$$PWD/robotModel/twoD/twoDRobotModel.cpp \

FORMS += \
	$$PWD/nxtAdditionalPreferences.ui \

include($$PWD/../qrguiIncludes.pri)
