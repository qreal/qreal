TARGET = qrgui-thirdparty

include(../../global.pri)

TEMPLATE = lib

DEFINES += QRGUI_THIRDPARTY_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_thirdparty_ru.ts

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

include($$PWD/qtpropertybrowser/src/qtpropertybrowser.pri)
