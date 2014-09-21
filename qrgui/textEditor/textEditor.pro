TEMPLATE = lib

DESTDIR = $$PWD/../../bin

LIBS += -L$$PWD/../../bin -lqrutils -lqscintilla2 -ltoolPluginInterface

QT += widgets

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

HEADERS += \
	$$PWD/codeEditor.h \
	$$PWD/textHighlighter.h \
	$$PWD/sqlHighlighter.h \
	$$PWD/textManager.h \
	$$PWD/textManagerInterface.h \

SOURCES += \
	$$PWD/codeEditor.cpp \
	$$PWD/textHighlighter.cpp \
	$$PWD/sqlHighlighter.cpp \
	$$PWD/textManager.cpp \
