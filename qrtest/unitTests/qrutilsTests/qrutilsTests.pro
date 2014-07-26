TARGET = qrutils_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrutils/qrutils.pri)

HEADERS += \
#	expressionsParser/expressionsParserTest.h \
	textLanguageParser/details/lexerTest.h \
#	metamodelGeneratorSupportTest.h \

SOURCES += \
#	expressionsParser/expressionsParserTest.cpp \
#	expressionsParser/numberTest.cpp \
	textLanguageParser/details/lexerTest.cpp \
#	metamodelGeneratorSupportTest.cpp \
#	inFileTest.cpp \
#	outFileTest.cpp \
#	xmlUtilsTest.cpp \
