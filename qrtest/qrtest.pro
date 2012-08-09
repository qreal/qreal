DESTDIR = ../bin/unittests
TARGET = qrlibs_unittests

!macx {
	QMAKE_LFLAGS += -Wl,-rpath,$(PWD)/../bin
	QMAKE_LFLAGS += -Wl,-rpath,$(PWD)/../bin/plugins
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += ../thirdparty/gmock-1.6.0/include
INCLUDEPATH += ../thirdparty/gmock-1.6.0/gtest/include

LIBS += -L../bin/ -lqrkernel -lqrutils -lqrmc -lqrrepo
LIBS += -L../bin/plugins/ -lvisualDebugSupport
LIBS += -L../thirdparty/ -lgmock_main -lpthread

HEADERS += \
	exampleTests/sampleFixtureTest.h \
	exampleTests/testClass.h \
	exampleTests/mockedTestClass.h \

SOURCES += \
	exampleTests/sampleTest.cpp \
	exampleTests/sampleFixtureTest.cpp \
	exampleTests/mockTest.cpp \

# Mocked classes
include(mocks/mocks.pri)

# Unit tests for kernel
include(qrkernelTests/qrkernelTests.pri)

# Unit tests for repo
include(qrrepoTests/qrrepoTests.pri)

# Unit tests for utils
include(qrutilsTests/qrutilsTests.pri)
