TARGET = qrgui_unittests

CONFIG += c++11

INCLUDEPATH += \
	# A little hack to make .ui files happy. They include other files by relative path based on qrgui/.ui \
	../../../qrgui/icons \
	../../../qrgui/ \

include(../common.pri)

include(../../../qrgui/qrgui.pri)

include(modelsTests/modelsTests.pri)

include(helpers/helpers.pri)
