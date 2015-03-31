TARGET = robots_twoDModel_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/common/twoDModel/twoDModel.pri)

links(qslog qrgui-preferences-dialog)

INCLUDEPATH += \
	../../../../../../plugins/robots/common/twoDModel \
	../../../../../../plugins/robots/common/twoDModel/include \

# Tests
HEADERS += \
	$$PWD/engineTests/constrasTests/constraintsParserTests.h \

SOURCES += \
	$$PWD/engineTests/constraintsTests/constraintsParserTests.cpp \

# Support classes
HEADERS += \
	$$PWD/support/testTimeline.h \
	$$PWD/support/testObject.h \

SOURCES += \
	$$PWD/support/testTimeline.cpp \
	$$PWD/support/testObject.cpp \
