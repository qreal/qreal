TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-text-editor

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils -lqscintilla2 -lqrgui-tool-plugin-interface

QT += widgets

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_TEXT_EDITOR_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_textEditor_ru.ts

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
