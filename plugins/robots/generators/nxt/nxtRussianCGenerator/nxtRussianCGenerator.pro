TARGET = robots-nxt-russian-c-generator

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

links(qrkernel qrutils qrrepo qscintilla2 robots-generator-base robots-nxt-generator-base)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtRussianCGenerator_ru.ts

HEADERS += \
	$$PWD/nxtRussianCGeneratorPlugin.h \
	$$PWD/nxtRussianCMasterGenerator.h \

SOURCES += \
	$$PWD/nxtRussianCGeneratorPlugin.cpp \
	$$PWD/nxtRussianCMasterGenerator.cpp \

RESOURCES = \
	$$PWD/nxtRussianCGenerator.qrc \
	$$PWD/templates.qrc \
