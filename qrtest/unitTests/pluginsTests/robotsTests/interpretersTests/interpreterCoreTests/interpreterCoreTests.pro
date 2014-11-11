TARGET = robots_interpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterCore/interpreterCore.pri)

LIBS += -L../../../../../../bin -lqrkernel -lqslog

INCLUDEPATH += \
	$$PWD/../../../../../../plugins/robots/interpreters \
	$$PWD/../../../../../../plugins/robots/interpreters/interpreterCore \
	$$PWD/../../../../../../plugins/robots/interpreters/interpreterBase/include/ \
	$$PWD/../../../../mocks/plugins/robots/interpreters/interpreterBase/include \
	$$PWD/../../../../mocks/plugins/robots/interpreters \

# Tests
HEADERS += \
	kitPluginManagerTest.h \
	interpreterTests/interpreterTest.h \
	interpreterTests/detailsTests/blocksTableTest.h \
	managersTests/sensorsConfigurationManagerTest.h \
	support/dummySensorsConfigurer.h \

SOURCES += \
	kitPluginManagerTest.cpp \
	interpreterTests/interpreterTest.cpp \
	interpreterTests/detailsTests/blocksTableTest.cpp \
	managersTests/sensorsConfigurationManagerTest.cpp \
	support/dummySensorsConfigurer.cpp \

# Mocks
include(mocks.pri)

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
	QMAKE_POST_LINK = "mkdir -p ../../../../../../bin/unittests && cp -r ../support/testData/* ../../../../../../bin/unittests/"
}
