TARGET = robots-nxt-osek-c-generator

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-nxt-generator-base)

INCLUDEPATH += \
	$$PWD/../nxtGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

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
