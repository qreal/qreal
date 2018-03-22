

TARGET = semantic-tree-tests

include($$PWD/../../../../common.pri)
include($$PWD/../../../../../../plugins/robots/generators/generatorBase/generatorBase.pri)

message($$PWD/../../../../../../plugins/robots/generators/generatorBase/generatorBase.pri)

links(test-utils qrgui-tool-plugin-interface qrgui-models qrgui-plugin-manager qrgui-text-editor qrtext \
		qrgui-controller tcp-robot-simulator)

QT += xml

HEADERS += \
	$$PWD/semanticTreeTest.h \


SOURCES += \
	$$PWD/semanticTreeTest.cpp \


copyToDestdir($$PWD/support/testData/unittests, NOW)
