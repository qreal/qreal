win32 {
	HEADERS += \
		thirdparty/windowsmodernstyle.h \

	SOURCES += \
		thirdparty/windowsmodernstyle.cpp \
}

HEADERS += \
	thirdparty/qsKineticScroller.h \

SOURCES += \
	thirdparty/qsKineticScroller.cpp \

include (qtpropertybrowser/src/qtpropertybrowser.pri)
