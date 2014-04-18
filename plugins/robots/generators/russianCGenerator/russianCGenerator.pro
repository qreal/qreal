QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../../bin -lqrkernel -lqrutils -lqrrepo -lgeneratorBase

INCLUDEPATH += \
	$$PWD/../generatorBase/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../bin/plugins/
}

TRANSLATIONS = russianCGenerator_ru.ts

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
