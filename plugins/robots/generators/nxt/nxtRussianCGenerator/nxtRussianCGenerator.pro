TARGET = robots-nxt-russian-c-generator

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

links(qrkernel qrutils qrrepo qscintilla2 robots-generator-base robots-nxt-generator-base)

INCLUDEPATH += \
	$$PWD/../nxtGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

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
