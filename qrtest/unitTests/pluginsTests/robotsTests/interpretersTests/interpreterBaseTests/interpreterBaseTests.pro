TARGET = robots_interpreterBase_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterBase/interpreterBase.pri)

LIBS += -L../../../../../../bin -lqslog

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters/interpreterBase \

# Tests
HEADERS += \
	robotModelTests/defaultRobotModelTest.h \
	robotModelTests/commonRobotModelTest.h \
	robotModelTests/configurationTest.h \
	robotModelTests/robotPartsTests/deviceTest.h \
	robotModelTests/deviceInfoTest.h \

SOURCES += \
	robotModelTests/defaultRobotModelTest.cpp \
	robotModelTests/commonRobotModelTest.cpp \
	robotModelTests/configurationTest.cpp \
	robotModelTests/robotPartsTests/deviceTest.cpp \
	robotModelTests/deviceInfoTest.cpp \

# Support classes
HEADERS += \
	support/dummyDevice.h \

SOURCES += \
	support/dummyDevice.cpp \
