TARGET = robots-trik-qts-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base robots-utils)

INCLUDEPATH += \
	$$PWD/../trikGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikQtsGenerator_ru.ts

HEADERS += \
	$$PWD/trikQtsGeneratorPlugin.h \
	$$PWD/trikQtsMasterGenerator.h \
	$$PWD/trikQtsControlFlowValidator.h \
	$$PWD/threadsValidator.h \

SOURCES += \
	$$PWD/trikQtsGeneratorPlugin.cpp \
	$$PWD/trikQtsMasterGenerator.cpp \
	$$PWD/trikQtsControlFlowValidator.cpp \
	$$PWD/threadsValidator.cpp \

RESOURCES = \
	$$PWD/trikQtsGenerator.qrc \
	$$PWD/templates.qrc \
