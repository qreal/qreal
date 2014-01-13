QT += widgets

INCLUDEPATH += \
	../interpreterBase/include \
	../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L../../../../bin -linterpreterBase


HEADERS += \
	$$PWD/nullKitInterpreterInterface.h \
	$$PWD/nullRobotModel.h \

SOURCES += \
	$$PWD/nullKitInterpreterInterface.cpp \
	$$PWD/nullRobotModel.cpp \
