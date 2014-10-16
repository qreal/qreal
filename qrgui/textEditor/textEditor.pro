TEMPLATE = lib

DESTDIR = $$PWD/../../bin

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils -lqscintilla2 -ltoolPluginInterface

QT += widgets

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_TEXT_EDITOR_LIBRARY

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

HEADERS += \
	$$PWD/textEditorDeclSpec.h \
	$$PWD/textHighlighter.h \
	$$PWD/sqlHighlighter.h \
	$$PWD/textManager.h \
	$$PWD/textManagerInterface.h \
	$$PWD/qscintillaTextEdit.h \

SOURCES += \
	$$PWD/textHighlighter.cpp \
	$$PWD/sqlHighlighter.cpp \
	$$PWD/textManager.cpp \
	$$PWD/qscintillaTextEdit.cpp \
