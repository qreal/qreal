

TARGET = semantic-tree-tests

include($$PWD/../../../../common.pri)

links(test-utils qrgui-tool-plugin-interface qrgui-models qrgui-plugin-manager qrgui-text-editor qrtext \
		qrgui-controller tcp-robot-simulator)


HEADERS += \
	$$PWD/semanticTreeTest.h \


SOURCES += \
	$$PWD/semanticTreeTest.cpp \


copyToDestdir($$PWD/support/testData/unittests, NOW)
