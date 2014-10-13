TARGET = qrtext_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrtext/qrtext.pri)

LIBS += -L../../../bin -lqslog

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
