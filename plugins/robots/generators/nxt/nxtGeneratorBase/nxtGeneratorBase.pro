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

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtGenerator_ru.ts

HEADERS += \
	$$PWD/include/nxtGeneratorBase/nxtGeneratorPluginBase.h \
	$$PWD/include/nxtGeneratorBase/nxtMasterGeneratorBase.h \
	$$PWD/src/nxtGeneratorCustomizer.h \
	$$PWD/src/nxtGeneratorFactory.h \
	$$PWD/src/converters/nxtStringPropertyConverter.h \

SOURCES += \
	$$PWD/src/nxtGeneratorPluginBase.cpp \
	$$PWD/src/nxtMasterGeneratorBase.cpp \
	$$PWD/src/nxtGeneratorCustomizer.cpp \
	$$PWD/src/nxtGeneratorFactory.cpp \
	$$PWD/src/converters/nxtStringPropertyConverter.cpp \
