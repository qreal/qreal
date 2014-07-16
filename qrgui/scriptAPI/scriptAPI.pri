QT += script testlib

HEADERS += \
	$$PWD/scriptAPI.h \
	$$PWD/arrow.h \
	$$PWD/virtualCursor.h \
	$$PWD/guiFacade.h \
	$$PWD/hintReporter.h \
    scriptAPI/paletteAPI.h \
    scriptAPI/hintAPI.h \
    scriptAPI/virtualKeyboard.h \
    scriptAPI/sceneAPI.h

SOURCES += \
	$$PWD/scriptAPI.cpp \
	$$PWD/arrow.cpp \
	$$PWD/virtualCursor.cpp \
	$$PWD/guiFacade.cpp \
	$$PWD/hintReporter.cpp \
    scriptAPI/paletteAPI.cpp \
    scriptAPI/hintAPI.cpp \
    scriptAPI/virtualKeyboard.cpp \
    scriptAPI/sceneAPI.cpp
