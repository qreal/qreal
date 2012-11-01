DEFINES += UNITTEST

TEMPLATE = app
CONFIG += console

QT += xml gui

DESTDIR = ../../bin/unittests
TARGET = qrlibs_unittests

!macx {
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../bin
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../bin/plugins
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	../../thirdparty/gmock-1.6.0/include \
	../../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L../../bin/unittests/ -lqrealSources
LIBS += -L../../bin/thirdparty/ -lgmock -lpthread

# Mocked classes
include(mocks/mocks.pri)

# Unit tests for kernel
include(qrkernelTests/qrkernelTests.pri)

# Unit tests for repo
include(qrrepoTests/qrrepoTests.pri)

# Unit tests for utils
include(qrutilsTests/qrutilsTests.pri)

# Unit tests for visual debug support plugin
include(visualDebugSupportPluginTests/visualDebugSupportPluginTests.pri)

# Example tests
include(exampleTests/exampleTests.pri)

OTHER_FILES += \
	unittestDeclaration.cfg \
