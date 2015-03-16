TARGET = robots-nxt-osek-c-generator

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/nxt/nxtGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-nxt-generator-base)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtOsekCGenerator_ru.ts

HEADERS += \
	$$PWD/nxtOsekCGeneratorPlugin.h \
	$$PWD/nxtOsekCMasterGenerator.h \
	$$PWD/nxtFlashTool.h \

SOURCES += \
	$$PWD/nxtOsekCGeneratorPlugin.cpp \
	$$PWD/nxtOsekCMasterGenerator.cpp \
	$$PWD/nxtFlashTool.cpp \

RESOURCES = \
	$$PWD/nxtOsekCGenerator.qrc \
	$$PWD/templates.qrc \
