TARGET = robots-trik-f-sharp-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

copyToDestdir(lib/Trik.Core.dll)

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

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikFSharpGenerator_ru.ts

HEADERS += \
	$$PWD/trikFSharpGeneratorPlugin.h \
	$$PWD/trikFSharpMasterGenerator.h \

SOURCES += \
	$$PWD/trikFSharpGeneratorPlugin.cpp \
	$$PWD/trikFSharpMasterGenerator.cpp \

RESOURCES = \
	$$PWD/trikFSharpGenerator.qrc \
	$$PWD/templates.qrc \
