HEADERS += \
	$$PWD/robotCommunicationThreadBase.h \
	$$PWD/bluetoothRobotCommunicationThread.h \
	$$PWD/usbRobotCommunicationThread.h \
	$$PWD/fantom.h \
	$$PWD/fantomMethods.h \
	$$PWD/i2cCommunicator.h \

SOURCES += \
	$$PWD/robotCommunicationThreadBase.cpp \
	$$PWD/bluetoothRobotCommunicationThread.cpp \
	$$PWD/usbRobotCommunicationThread.cpp \
	$$PWD/i2cCommunicator.cpp \

win32 {
	HEADERS += \
		$$PWD/windowsFantom.h \

	SOURCES += \
		$$PWD/windowsFantom.cpp \

}

unix {
	HEADERS += \
		$$PWD/linuxFantom.h \

	SOURCES += \
		$$PWD/linuxFantom.cpp \

}

macx {
	HEADERS += \
		$$PWD/macFantom.h \

	SOURCES += \
		$$PWD/macFantom.cpp \

}
