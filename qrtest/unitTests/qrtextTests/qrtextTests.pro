TARGET = qrtext_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrtext/qrtext.pri)

LIBS += -L../../../bin -lqslog

HEADERS += \
	luaLexerTest.h \
	luaParserTest.h \
	luaSemanticAnalyzerTest.h \
	luaToolboxTest.h \

SOURCES += \
	luaParserTest.cpp \
	luaSemanticAnalyzerTest.cpp \
	luaToolboxTest.cpp \
	luaLexerTest.cpp \
