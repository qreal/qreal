TARGET = robotsInterpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore/robotsInterpreterCore.pri)
include(../../../../../../qrgui/qrgui.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore \

HEADERS += \
	kitPluginManagerTest.h \
	interpreterTests/interpreterTest.h \
	robotModelTests/robotModelTest.h \

SOURCES += \
	kitPluginManagerTest.cpp \
	interpreterTests/interpreterTest.cpp \
	robotModelTests/robotModelTest.cpp \
