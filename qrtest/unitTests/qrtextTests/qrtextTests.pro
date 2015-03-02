TARGET = qrtext_unittests

include(../common.pri)

include(../../../qrtext/qrtext.pri)

links(qslog)

INCLUDEPATH += ../../../qrtext/include

HEADERS += \
	luaInterpreterIncorrectInputTest.h \
	luaInterpreterTest.h \
	luaLexerTest.h \
	luaParserIncorrectInputTest.h \
	luaParserTest.h \
	luaSemanticAnalyzerTest.h \
	luaToolboxTest.h \

SOURCES += \
	luaInterpreterIncorrectInputTest.cpp \
	luaInterpreterTest.cpp \
	luaParserIncorrectInputTest.cpp \
	luaParserTest.cpp \
	luaSemanticAnalyzerTest.cpp \
	luaToolboxTest.cpp \
	luaLexerTest.cpp \
