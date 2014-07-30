TARGET = visualDebugSupport_unittests

LIBS += -L../../../../../bin -lqslog

include(../../../common.pri)

include(../../../../../plugins/blockDiagram/visualDebugSupport/visualDebugSupport.pri)

SOURCES += \
	interpreter/blockParserTest.cpp \

HEADERS += \
	interpreter/blockParserTest.h \
