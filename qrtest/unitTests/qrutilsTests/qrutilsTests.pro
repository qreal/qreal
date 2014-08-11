TARGET = qrutils_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrutils/qrutils.pri)

LIBS += -L../../../bin -lqslog

HEADERS += \
#	expressionsParser/expressionsParserTest.h \
	textLanguageParser/textLanguageParserTest.h \
	textLanguageParser/semanticAnalyzerTest.h \
#	textLanguageParser/details/lexerTest.h \
#	metamodelGeneratorSupportTest.h \

SOURCES += \
#	expressionsParser/expressionsParserTest.cpp \
#	expressionsParser/numberTest.cpp \
	textLanguageParser/textLanguageParserTest.cpp \
	textLanguageParser/semanticAnalyzerTest.cpp \
#	textLanguageParser/details/lexerTest.cpp \
#	metamodelGeneratorSupportTest.cpp \
#	inFileTest.cpp \
#	outFileTest.cpp \
#	xmlUtilsTest.cpp \
