TARGET = qrgui_unittests

include(../common.pri)

links(qrkernel qslog qrutils qrrepo qrgui-models qrgui-controller qrgui-plugin-manager qrgui-tool-plugin-interface)

INCLUDEPATH += \
	# A little hack to make .ui files happy. They include other files by relative path based on qrgui/.ui \
	$$PWD/../../../qrgui/icons \
	$$PWD/../../../qrgui/ \

include(modelsTests/modelsTests.pri)

include(helpers/helpers.pri)
