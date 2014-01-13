QT += widgets

INCLUDEPATH += \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \

LIBS += -L$$PWD/../../../../../bin -linterpreterBase


HEADERS += \
	$$PWD/nullKitInterpreterInterface.h \
	$$PWD/nullRobotModel.h \

SOURCES += \
	$$PWD/nullKitInterpreterInterface.cpp \
	$$PWD/nullRobotModel.cpp \
