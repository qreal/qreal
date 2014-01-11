TARGET = robots_interpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterCore/interpreterCore.pri)
include(../../../../../../qrgui/qrgui.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters/interpreterCore \
	../../../../../../plugins/robots/interpreters/interpreterBase/include/ \
	../../../../mocks/plugins/robots/interpreters/interpreterBase/include \

# Tests
HEADERS += \
	kitPluginManagerTest.h \
	interpreterTests/interpreterTest.h \

SOURCES += \
	kitPluginManagerTest.cpp \
	interpreterTests/interpreterTest.cpp \

# Mocks
include(mocks.pri)

# InterpreterBase
#include(../../../../../../plugins/robots/interpreters/interpreterBase/include/interpreterBase.pri)

# Supporting code
HEADERS += \
	support/dummyBlock.h \
	support/dummyBlocksFactory.h \
	support/qrguiFacade.h \

SOURCES += \
	support/dummyBlock.cpp \
	support/dummyBlocksFactory.cpp \
	support/qrguiFacade.cpp \

win32 {
	QMAKE_POST_LINK = "cmd /C "xcopy ..\\support\\testData ..\\..\\..\\..\\..\\..\\bin\\unittests /s /e /q /y /i""
}
else {
	QMAKE_POST_LINK = "cp -r ../support/testData/* ../../../../../../bin/unittests/"
}
