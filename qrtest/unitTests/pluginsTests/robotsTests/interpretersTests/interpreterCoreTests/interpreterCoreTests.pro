TARGET = robots_interpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterCore/interpreterCore.pri)
include(../../../../../../qrgui/models/models.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters \
	../../../../../../plugins/robots/interpreters/interpreterCore \
	../../../../../../plugins/robots/interpreters/interpreterBase/include/ \
	../../../../mocks/plugins/robots/interpreters/interpreterBase/include \
	../../../../mocks/plugins/robots/interpreters \

#qrgui sources
HEADERS += \
	../../../../../../qrgui/mainwindow/mainWindowInterpretersInterface.h \
	../../../../../../qrgui/toolPluginInterface/customizer.h \
	../../../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h \
	../../../../../../qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h \
	../../../../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h \
	../../../../../../qrgui/pluginManager/editorManager.h \
	../../../../../../qrgui/pluginManager/listenerManager.h \
	../../../../../../qrgui/pluginManager/pattern.h \
	../../../../../../qrgui/pluginManager/explosion.h \
	../../../../../../qrgui/pluginManager/details/patternParser.h \
	../../../../../../qrgui/umllib/sdfRenderer.h \

SOURCES += \
	../../../../../../qrgui/pluginManager/editorManager.cpp \
	../../../../../../qrgui/pluginManager/listenerManager.cpp \
	../../../../../../qrgui/pluginManager/pattern.cpp \
	../../../../../../qrgui/pluginManager/explosion.cpp \
	../../../../../../qrgui/pluginManager/details/patternParser.cpp \
	../../../../../../qrgui/umllib/sdfRenderer.cpp \

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

LIBS += -L../../../../../../bin/ -lqslog

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
	QMAKE_POST_LINK = "mkdir -p ../../../../../../bin/unittests && cp -r ../support/testData/* ../../../../../../bin/unittests/"
}
