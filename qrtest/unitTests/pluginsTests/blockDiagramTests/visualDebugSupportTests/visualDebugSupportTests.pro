TARGET = visualDebugSupport_unittests

include(../../../common.pri)

include(../../../../../plugins/blockDiagram/visualDebugSupport/visualDebugSupport.pri)

SOURCES += \
	interpreter/blockParserTest.cpp \

HEADERS += \
	interpreter/blockParserTest.h \
