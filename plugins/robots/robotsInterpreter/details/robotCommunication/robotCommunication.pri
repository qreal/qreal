HEADERS += \
	details/robotCommunication/robotCommunicator.h \
	details/robotCommunication/robotCommunicationThreadInterface.h \
	details/robotCommunication/bluetoothRobotCommunicationThread.h \
	details/robotCommunication/usbRobotCommunicationThread.h \
	details/robotCommunication/fantom.h \
	details/robotCommunication/fantomMethods.h \
	details/robotCommunication/robotCommunicationException.h \

SOURCES += \
	details/robotCommunication/bluetoothRobotCommunicationThread.cpp \
	details/robotCommunication/usbRobotCommunicationThread.cpp \
	details/robotCommunication/robotCommunicator.cpp \
	details/robotCommunication/robotCommunicationException.cpp \

win32 {
	HEADERS += \
		details/robotCommunication/windowsFantom.h \

	SOURCES += \
		details/robotCommunication/windowsFantom.cpp \

}

unix {
	HEADERS += \
		details/robotCommunication/linuxFantom.h \

	SOURCES += \
		details/robotCommunication/linuxFantom.cpp \

}

macx {
	HEADERS += \
		details/robotCommunication/macFantom.h \

	SOURCES += \
		details/robotCommunication/macFantom.cpp \

}
