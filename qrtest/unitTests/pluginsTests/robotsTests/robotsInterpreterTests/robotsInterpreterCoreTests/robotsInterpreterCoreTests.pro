TARGET = robotsInterpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore/robotsInterpreterCore.pri)

HEADERS += \
	commonRobotModelTests/commonRobotModelTest.h \

SOURCES += \
	commonRobotModelTests/commonRobotModelTest.cpp \
