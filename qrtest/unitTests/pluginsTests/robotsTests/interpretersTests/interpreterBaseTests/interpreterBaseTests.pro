TARGET = robots_interpreterBase_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterBase/interpreterBase.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters/interpreterBase \

# Tests
HEADERS += \
	robotModelTests/robotModelTest.h \

SOURCES += \
	robotModelTests/robotModelTest.cpp \
