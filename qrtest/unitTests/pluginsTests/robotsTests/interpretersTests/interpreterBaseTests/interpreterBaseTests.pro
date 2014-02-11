TARGET = robots_interpreterBase_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterBase/interpreterBase.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters/interpreterBase \

# Tests
HEADERS += \
	robotModelTests/defaultRobotModelTest.h \
	robotModelTests/commonRobotModelTest.h \
	robotModelTests/configurationTest.h \
	robotModelTests/robotPartsTests/pluggableDeviceTest.h \
	robotModelTests/pluggableDeviceInfoTest.h \

SOURCES += \
	robotModelTests/defaultRobotModelTest.cpp \
	robotModelTests/commonRobotModelTest.cpp \
	robotModelTests/configurationTest.cpp \
	robotModelTests/robotPartsTests/pluggableDeviceTest.cpp \
	robotModelTests/pluggableDeviceInfoTest.cpp \

# Support classes
HEADERS += \
	support/dummyPluggableDevice.h \

SOURCES += \
	support/dummyPluggableDevice.cpp \
