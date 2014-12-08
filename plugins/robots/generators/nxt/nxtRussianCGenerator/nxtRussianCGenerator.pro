QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-nxt-russian-c-generator

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqrutils -lqrrepo -lqscintilla2 \
		-lrobots-generator-base -lrobots-nxt-generator-base \

INCLUDEPATH += \
	$$PWD/../nxtGeneratorBase/include/ \
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
