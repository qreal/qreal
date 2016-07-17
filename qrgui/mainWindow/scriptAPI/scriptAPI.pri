QT += script testlib
QT.testlib.CONFIG -= console

HEADERS += \
	$$PWD/scriptAPI.h \
	$$PWD/arrow.h \
	$$PWD/virtualCursor.h \
	$$PWD/guiFacade.h \
	$$PWD/hintReporter.h \
	$$PWD/paletteAPI.h \
	$$PWD/hintAPI.h \
	$$PWD/virtualKeyboard.h \
	$$PWD/sceneAPI.h \

SOURCES += \
	$$PWD/scriptAPI.cpp \
	$$PWD/arrow.cpp \
	$$PWD/virtualCursor.cpp \
	$$PWD/guiFacade.cpp \
	$$PWD/hintReporter.cpp \
	$$PWD/paletteAPI.cpp \
	$$PWD/hintAPI.cpp \
	$$PWD/virtualKeyboard.cpp \
	$$PWD/sceneAPI.cpp \
