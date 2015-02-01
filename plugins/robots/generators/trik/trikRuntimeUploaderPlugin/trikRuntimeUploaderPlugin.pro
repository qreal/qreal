TARGET = robots-trik-runtime-uploader-plugin

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

links(qrkernel qslog qrutils qrrepo robots-generator-base robots-trik-generator-base)

INCLUDEPATH += \
	$$PWD/../trikGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikRuntimeUploaderPlugin_ru.ts

HEADERS += \
	$$PWD/trikRuntimeUploaderPlugin.h \

SOURCES += \
	$$PWD/trikRuntimeUploaderPlugin.cpp \

RESOURCES = \
	$$PWD/trikRuntimeUploaderPlugin.qrc \
