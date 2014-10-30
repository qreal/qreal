QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/
TARGET = robots-nxt-generator-base

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo -lrobots-generator-base

DEFINES += ROBOTS_NXT_GENERATOR_BASE_LIBRARY

INCLUDEPATH += \
	$$PWD/include \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../../bin/plugins/
}

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
	$$PWD/src/converters/nxtStringPropertyConverter.h \
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
	$$PWD/src/converters/nxtStringPropertyConverter.cpp \
	$$PWD/src/converters/colorConverter.cpp \
