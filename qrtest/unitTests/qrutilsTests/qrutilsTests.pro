TARGET = qrutils_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrutils/qrutils.pri)

LIBS += -L../../../bin -lqslog

HEADERS += \
	expressionsParser/expressionsParserTest.h \
	metamodelGeneratorSupportTest.h \

SOURCES += \
	expressionsParser/expressionsParserTest.cpp \
	expressionsParser/numberTest.cpp \
	metamodelGeneratorSupportTest.cpp \
	inFileTest.cpp \
	outFileTest.cpp \
	xmlUtilsTest.cpp \
