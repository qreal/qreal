TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

HEADERS = \
	visualDebuggerPlugin.h \
	visualDebuggerPreferencesPage.h \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h

SOURCES = \
	visualDebuggerPlugin.cpp \
	visualDebuggerPreferencesPage.cpp

FORMS += \
	visualDebuggerPreferencesPage.ui

RESOURCES += \
	visualDebugger.qrc

include(interpreter/interpreter.pri)
