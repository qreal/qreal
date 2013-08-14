win32 {
	HEADERS += \
		$$PWD/windowsmodernstyle.h \

	SOURCES += \
		$$PWD/windowsmodernstyle.cpp \
}

HEADERS += \
	$$PWD/qsKineticScroller.h \

SOURCES += \
	$$PWD/qsKineticScroller.cpp \

include ($$PWD/qtpropertybrowser/src/qtpropertybrowser.pri)
