TARGET = qrutils_unittests

include(../common.pri)

include(../../../qrutils/qrutils.pri)

links(qslog)

INCLUDEPATH += ../../../qrutils

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
