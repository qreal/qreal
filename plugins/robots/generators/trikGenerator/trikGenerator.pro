QT += widgets network

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../../bin/plugins/kitPlugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../../bin -lqrkernel -lqrutils -lqrrepo -lgeneratorBase

INCLUDEPATH += \
	$$PWD/../generatorBase/include/ \
	$$PWD/../../interpreters/interpreterBase/include \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../bin/plugins/
}

TRANSLATIONS = trikGenerator_ru.ts

RESOURCES = \
	$$PWD/trikGenerator.qrc \
	$$PWD/templates.qrc \

HEADERS += \
	$$PWD/trikGeneratorPlugin.h \
	$$PWD/trikMasterGenerator.h \
	$$PWD/trikGeneratorCustomizer.h \
	$$PWD/trikGeneratorFactory.h \
	$$PWD/parts/trikVariables.h \
	$$PWD/converters/trikEnginePortsConverter.h \
	$$PWD/simpleGenerators/trikEnginesGenerator.h \
	$$PWD/simpleGenerators/trikEnginesStopGenerator.h

SOURCES += \
	$$PWD/trikGeneratorPlugin.cpp \
	$$PWD/trikMasterGenerator.cpp \
	$$PWD/trikGeneratorCustomizer.cpp \
	$$PWD/trikGeneratorFactory.cpp \
	$$PWD/parts/trikVariables.cpp \
	$$PWD/converters/trikEnginePortsConverter.cpp \
	$$PWD/simpleGenerators/trikEnginesGenerator.cpp \
	$$PWD/simpleGenerators/trikEnginesStopGenerator.cpp \

include(robotCommunication/robotCommunication.pri)
