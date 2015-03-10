TARGET = robots-trik-qts-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base robots-utils)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikQtsGenerator_ru.ts

HEADERS += \
	$$PWD/trikQtsGeneratorPlugin.h \
	$$PWD/trikQtsMasterGenerator.h \

SOURCES += \
	$$PWD/trikQtsGeneratorPlugin.cpp \
	$$PWD/trikQtsMasterGenerator.cpp \

RESOURCES = \
	$$PWD/trikQtsGenerator.qrc \
	$$PWD/templates.qrc \
