QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-nxt-russian-c-generator

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../../bin -lqrkernel -lqrutils -lqrrepo -lrobots-generator-base

INCLUDEPATH += \
	$$PWD/../generatorBase/include/ \
	$$PWD/../../interpreters/interpreterBase/include \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui \
	$$PWD/../../../../qrtext/include \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../bin/plugins/
}

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/russianCGenerator_ru.ts

RESOURCES = \
	$$PWD/russianCGenerator.qrc \
	$$PWD/templates.qrc \

HEADERS += \
	$$PWD/russianCGeneratorPlugin.h \
	$$PWD/russianCMasterGenerator.h \
	$$PWD/russianCGeneratorCustomizer.h \
	$$PWD/russianCGeneratorFactory.h \
	$$PWD/converters/russianCStringPropertyConverter.h \

SOURCES += \
	$$PWD/russianCGeneratorPlugin.cpp \
	$$PWD/russianCMasterGenerator.cpp \
	$$PWD/russianCGeneratorCustomizer.cpp \
	$$PWD/russianCGeneratorFactory.cpp \
	$$PWD/converters/russianCStringPropertyConverter.cpp \
