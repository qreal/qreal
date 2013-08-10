QT += widgets

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = $$PWD/visualDebugSupport_ru.ts

HEADERS = \
	$$PWD/visualDebuggerPlugin.h \
	$$PWD/visualDebuggerPreferencesPage.h \
	$$PWD/../../../qrgui/dialogs/preferencesPages/preferencesPage.h \

SOURCES = \
	$$PWD/visualDebuggerPlugin.cpp \
	$$PWD/visualDebuggerPreferencesPage.cpp \

FORMS += \
	$$PWD/visualDebuggerPreferencesPage.ui \

RESOURCES += \
	$$PWD/visualDebugger.qrc \

include($$PWD/interpreter/interpreter.pri)
