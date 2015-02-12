TARGET = qrgui-text-editor

include(../../global.pri)

TEMPLATE = lib

links(qrkernel qrutils qscintilla2 qrgui-tool-plugin-interface)

QT += widgets

DEFINES += QRGUI_TEXT_EDITOR_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_textEditor_ru.ts

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \
	$$PWD/../../thirdparty/qscintilla/Qt4Qt5 \

HEADERS += \
	$$PWD/textEditorDeclSpec.h \
	$$PWD/textHighlighter.h \
	$$PWD/sqlHighlighter.h \
	$$PWD/textManager.h \
	$$PWD/textManagerInterface.h \
	$$PWD/qscintillaTextEdit.h \
	$$PWD/languageInfo.h \

SOURCES += \
	$$PWD/textHighlighter.cpp \
	$$PWD/sqlHighlighter.cpp \
	$$PWD/textManager.cpp \
	$$PWD/qscintillaTextEdit.cpp \
	$$PWD/languageInfo.cpp \
