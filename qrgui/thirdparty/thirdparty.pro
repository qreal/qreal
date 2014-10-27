TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-thirdparty

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_THIRDPARTY_LIBRARY

win32 {
	HEADERS += \
		$$PWD/windowsmodernstyle.h \

	SOURCES += \
		$$PWD/windowsmodernstyle.cpp \
}

HEADERS += \
	$$PWD/thirdPartyDeclSpec.h \
	$$PWD/qsKineticScroller.h \

SOURCES += \
	$$PWD/qsKineticScroller.cpp \

include ($$PWD/qtpropertybrowser/src/qtpropertybrowser.pri)
