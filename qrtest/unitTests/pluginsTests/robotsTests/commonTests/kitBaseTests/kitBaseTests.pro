TARGET = robots_kitBase_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/common/kitBase/kitBase.pri)

links(qslog)

INCLUDEPATH += \
	../../../../../../plugins/robots/common/kitBase \
	../../../../../../plugins/robots/common/kitBase/include \

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
