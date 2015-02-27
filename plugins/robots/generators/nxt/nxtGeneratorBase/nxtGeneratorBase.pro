TARGET = robots-nxt-generator-base

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

includes(plugins/robots/generators/nxt/nxtGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo robots-generator-base)

DEFINES += ROBOTS_NXT_GENERATOR_BASE_LIBRARY

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtGeneratorBase_ru.ts

HEADERS += \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorPluginBase.h \
	$$PWD/include/nxtGeneratorBase/nxtMasterGeneratorBase.h \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorFactory.h \
	$$PWD/include/nxtGeneratorBase/parts/images.h \
	$$PWD/src/nxtGeneratorCustomizer.h \
	$$PWD/src/simpleGenerators/clearScreenBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawPixelBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawRectBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawLineBlockGenerator.h \
	$$PWD/src/simpleGenerators/drawCircleBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForColorBlockGenerator.h \
	$$PWD/src/converters/colorConverter.h \

SOURCES += \
	$$PWD/src/nxtGeneratorPluginBase.cpp \
	$$PWD/src/nxtMasterGeneratorBase.cpp \
	$$PWD/src/nxtGeneratorCustomizer.cpp \
	$$PWD/src/nxtGeneratorFactory.cpp \
	$$PWD/src/parts/images.cpp \
	$$PWD/src/simpleGenerators/clearScreenBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawPixelBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawRectBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawLineBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/drawCircleBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForColorBlockGenerator.cpp \
	$$PWD/src/converters/colorConverter.cpp \
