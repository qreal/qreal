TARGET = 2D-model

include(../../../../global.pri)

TEMPLATE = app

QT += widgets

INCLUDEPATH += \
	$$PWD/../interpreterCore/include/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../commonTwoDModel/include/ \
	$$PWD/../../generators/generatorBase/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../qrgui/ \
	$$PWD/../../../../qrtext/include \

links(qslog qrkernel qrutils qrgui-tool-plugin-interface qrgui-preferences-dialog qrgui-facade \
		qrgui-models qrgui-editor qrgui-plugin-manager qrgui-text-editor \
		robots-utils robots-interpreter-base robots-interpreter-core robots-common-2d-model \
)

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/twoDModelRunner_ru.ts

HEADERS += \
	$$PWD/runner.h \

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/runner.cpp \
