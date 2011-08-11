# Style for GUI
win32{
	HEADERS += \
		thirdparty/windowsmodernstyle.h \

	SOURCES += \
		thirdparty/windowsmodernstyle.cpp \
}

# Serial port programming library (qextserialport.sourceforge.net)
HEADERS += \
	thirdparty/qextserialport/src/qextserialenumerator.h \
	thirdparty/qextserialport/src/qextserialport.h \
	thirdparty/qextserialport/src/qextserialport_global.h \


include (qtpropertybrowser/src/qtpropertybrowser.pri)
