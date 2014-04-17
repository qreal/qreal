QT += script

HEADERS += \
	$$PWD/scriptAPI.h \
	$$PWD/arrow.h \
	$$PWD/virtualCursor.h \
	$$PWD/guiFacade.h \
    scriptAPI/hintReporter.h

SOURCES += \
	$$PWD/scriptAPI.cpp \
	$$PWD/arrow.cpp \
	$$PWD/virtualCursor.cpp \
	$$PWD/guiFacade.cpp \
    scriptAPI/hintReporter.cpp
