include(../common.pri)

links(qrkernel qrutils qslog)

include(../../../plugins/blockDiagram/visualDebugSupport/visualDebugSupport.pri)

HEADERS += \
	sampleFixtureTest.h \
	testClass.h \
	mockedTestClass.h \

SOURCES += \
	sampleTest.cpp \
	sampleFixtureTest.cpp \
	mockTest.cpp \
