TARGET = robots-trik-runtime-uploader-plugin

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo robots-generator-base robots-trik-generator-base)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikRuntimeUploaderPlugin_ru.ts

HEADERS += \
	$$PWD/trikRuntimeUploaderPlugin.h \

SOURCES += \
	$$PWD/trikRuntimeUploaderPlugin.cpp \

RESOURCES = \
	$$PWD/trikRuntimeUploaderPlugin.qrc \
