TARGET = 2D-model

include(../../../../global.pri)

TEMPLATE = app

QT += widgets

includes(plugins/robots/interpreters/interpreterCore \
		plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/generators/generatorBase/ \ # TODO: REMOVE!!!
		plugins/robots/utils \
		qrtext \
)

links(qslog qrkernel qrutils qrgui-tool-plugin-interface qrgui-preferences-dialog qrgui-facade \
		qrgui-models qrgui-editor qrgui-plugin-manager qrgui-text-editor \
		robots-utils robots-kit-base robots-interpreter-core robots-2d-model \
)

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/twoDModelRunner_ru.ts

HEADERS += \
	$$PWD/runner.h \

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/runner.cpp \
