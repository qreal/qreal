TARGET = robots_commonTwoDModel_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/commonTwoDModel/commonTwoDModel.pri)

links(qslog qrgui-preferences-dialog)

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters/commonTwoDModel \
	../../../../../../plugins/robots/interpreters/commonTwoDModel/include \

# Tests
HEADERS += \
	$$PWD/engineTests/constraintsTests/constraintsParserTests.h \

SOURCES += \
	$$PWD/engineTests/constraintsTests/constraintsParserTests.cpp \

# Support classes
HEADERS += \
	$$PWD/support/testTimeline.h \
	$$PWD/support/testObject.h \

SOURCES += \
	$$PWD/support/testTimeline.cpp \
	$$PWD/support/testObject.cpp \
