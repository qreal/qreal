QT += widgets

INCLUDEPATH += \
	$$PWD/../include \
	$$PWD/../../../../ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrutils

DEFINES += ROBOTS_UTILS_LIBRARY

HEADERS += \
	$$PWD/../include/utils/utils.h \
	$$PWD/../include/utils/timelineInterface.h \
	$$PWD/../include/utils/abstractTimer.h \
	$$PWD/../include/utils/realTimeline.h \
	$$PWD/../include/utils/realTimer.h \
	$$PWD/../include/utils/textExpressionProcessor.h \
	$$PWD/../include/utils/tracer.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicationThreadInterface.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicator.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicationException.h \

SOURCES += \
	$$PWD/utils.cpp \
	$$PWD/abstractTimer.cpp \
	$$PWD/realTimeline.cpp \
	$$PWD/realTimer.cpp \
	$$PWD/textExpressionProcessor.cpp \
	$$PWD/tracer.cpp \
	$$PWD/robotCommunication/robotCommunicator.cpp \
	$$PWD/robotCommunication/robotCommunicationException.cpp \
