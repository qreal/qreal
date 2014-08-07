TARGET = exampleTests

CONFIG += c++11

include(../common.pri)

LIBS += -L../../../bin -lqrkernel -lqrutils -lqslog

include(../../../plugins/blockDiagram/visualDebugSupport/visualDebugSupport.pri)

HEADERS += \
	sampleFixtureTest.h \
	testClass.h \
	mockedTestClass.h \

SOURCES += \
	sampleTest.cpp \
	sampleFixtureTest.cpp \
	mockTest.cpp \
