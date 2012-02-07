TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS = \
	visualDebuggerPlugin.h \
	visualDebuggerPreferencesPage.h \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h

SOURCES = \
	visualDebuggerPlugin.cpp \
	visualDebuggerPreferencesPage.cpp

FORMS += \
	visualDebuggerPreferencesPage.ui

include(interpreter/interpreter.pri)

RESOURCES += \
	visualDebugger.qrc
