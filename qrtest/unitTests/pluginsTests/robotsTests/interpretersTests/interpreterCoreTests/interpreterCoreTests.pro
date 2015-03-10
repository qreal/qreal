TARGET = robots_interpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterCore/interpreterCore.pri)

links(qrkernel qslog)

INCLUDEPATH += \
	$$PWD/../../../../../../plugins/robots/interpreters \
	$$PWD/../../../../../../plugins/robots/interpreters/interpreterCore \
	$$PWD/../../../../../../plugins/robots/common/kitBase/include \
	$$PWD/../../../../mocks/plugins/robots/common/kitBase/include \
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

copyToDestdir(../support/testData/unittests, NOW)
