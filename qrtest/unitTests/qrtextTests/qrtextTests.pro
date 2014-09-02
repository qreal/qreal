TARGET = qrtext_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrtext/qrtext.pri)

LIBS += -L../../../bin -lqslog

HEADERS += \
	luaInterpreterTest.h \
#	luaLexerTest.h \
#	luaParserTest.h \
	luaSemanticAnalyzerTest.h \
	luaToolboxTest.h \

SOURCES += \
	luaInterpreterTest.cpp \
#	luaParserTest.cpp \
	luaSemanticAnalyzerTest.cpp \
	luaToolboxTest.cpp \
#	luaLexerTest.cpp \
