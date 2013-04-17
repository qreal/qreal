TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

QT += widgets

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = visualDebugSupport_ru.ts

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

include(interpreter/interpreter.pri)
