TARGET = robotsInterpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore/robotsInterpreterCore.pri)
include(../../../../../../qrgui/qrgui.pri)

INCLUDEPATH += \
	../../../../../../plugins/robots/robotsInterpreter/robotsInterpreterCore \

# Tests
HEADERS += \
	kitPluginManagerTest.h \
	interpreterTests/interpreterTest.h \
	robotModelTests/robotModelTest.h \

SOURCES += \
	kitPluginManagerTest.cpp \
	interpreterTests/interpreterTest.cpp \
	robotModelTests/robotModelTest.cpp \

# Mocks
HEADERS += \
	../../../../mocks/qrgui/mainwindow/mainWindowInterpretersInterfaceMock.h \
	../../../../mocks/qrgui/mainwindow/projectManager/projectManagementInterfaceMock.h \
	../../../../mocks/qrgui/view/sceneCustomizationInterfaceMock.h \

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
	QMAKE_POST_LINK = "xcopy ..\\testData\\* ..\\..\\..\\..\\..\\..\\bin\\unittests\\ /q /y"
}
else {
	QMAKE_POST_LINK = "cp ../testData/* ../../../../../../bin/unittests/"
}
