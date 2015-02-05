TARGET = robots_commonTwoDModel_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/commonTwoDModel/commonTwoDModel.pri)

LIBS += -L../../../../../../bin -lqslog -lqrgui-preferences-dialog

INCLUDEPATH += \
	../../../../../../plugins/robots/interpreters/commonTwoDModel \

# Tests
HEADERS += \
	$$PWD/engineTests/constraintsTests/constraintsParserTests.h \

SOURCES += \
	$$PWD/engineTests/constraintsTests/constraintsParserTests.cpp \
