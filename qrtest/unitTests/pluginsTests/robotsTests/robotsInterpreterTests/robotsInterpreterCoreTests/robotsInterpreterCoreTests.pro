TARGET = robotsInterpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore/robotsInterpreterCore.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore \

HEADERS += \
	kitPluginManagerTest.h \
	commonRobotModelTests/commonRobotModelTest.h \

SOURCES += \
	kitPluginManagerTest.cpp \
	commonRobotModelTests/commonRobotModelTest.cpp \
