TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = visualDebugSupport_ru.ts

HEADERS = \
	$$PWD/visualDebuggerPlugin.h \
	$$PWD/visualDebuggerPreferencesPage.h \
	$$PWD/../../../qrgui/dialogs/preferencesPages/preferencesPage.h

SOURCES = \
	$$PWD/visualDebuggerPlugin.cpp \
	$$PWD/visualDebuggerPreferencesPage.cpp

FORMS += \
	$$PWD/visualDebuggerPreferencesPage.ui

RESOURCES += \
	$$PWD/visualDebugger.qrc

include(interpreter/interpreter.pri)
