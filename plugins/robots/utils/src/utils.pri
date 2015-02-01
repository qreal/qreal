QT += widgets network

links(qslog qrkernel qrutils)

DEFINES += ROBOTS_UTILS_LIBRARY

HEADERS += \
	$$PWD/../include/utils/timelineInterface.h \
	$$PWD/../include/utils/abstractTimer.h \
	$$PWD/../include/utils/realTimeline.h \
	$$PWD/../include/utils/realTimer.h \
	$$PWD/../include/utils/tracer.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicationThreadInterface.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicator.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicationException.h \
	$$PWD/../include/utils/requiredVersion.h \
	$$PWD/../include/utils/tcpRobotCommunicator.h \
	$$PWD/../include/utils/tcpConnectionHandler.h

SOURCES += \
	$$PWD/abstractTimer.cpp \
	$$PWD/realTimeline.cpp \
	$$PWD/realTimer.cpp \
	$$PWD/tracer.cpp \
	$$PWD/robotCommunication/robotCommunicator.cpp \
	$$PWD/robotCommunication/robotCommunicationException.cpp \
	$$PWD/tcpRobotCommunicator.cpp \
	$$PWD/tcpConnectionHandler.cpp \

TRANSLATIONS += $$PWD/../../../../qrtranslations/ru/plugins/robots/robots_utils_ru.ts
