QT += widgets network

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo -lrobotsGeneratorBase

INCLUDEPATH += \
	$$PWD/../robotsGeneratorBase/ \
	$$PWD/../../../ \
	$$PWD/../../../qrgui \

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
	$$PWD/converters/backgroundColorConverter.h \
	$$PWD/converters/ledColorConverter.h \
	$$PWD/converters/trikEnginePortsConverter.h \
	$$PWD/simpleGenerators/ledGenerator.h \
	$$PWD/simpleGenerators/sadSmileGenerator.h \
	$$PWD/simpleGenerators/setBackgroundGenerator.h \
	$$PWD/simpleGenerators/smileGenerator.h \
	$$PWD/simpleGenerators/trikEnginesGenerator.h \
	$$PWD/simpleGenerators/trikEnginesStopGenerator.h \

SOURCES += \
	$$PWD/trikGeneratorPlugin.cpp \
	$$PWD/trikMasterGenerator.cpp \
	$$PWD/trikGeneratorCustomizer.cpp \
	$$PWD/trikGeneratorFactory.cpp \
	$$PWD/parts/trikVariables.cpp \
	$$PWD/converters/backgroundColorConverter.cpp \
	$$PWD/converters/ledColorConverter.cpp \
	$$PWD/converters/trikEnginePortsConverter.cpp \
	$$PWD/simpleGenerators/ledGenerator.cpp \
	$$PWD/simpleGenerators/sadSmileGenerator.cpp \
	$$PWD/simpleGenerators/setBackgroundGenerator.cpp \
	$$PWD/simpleGenerators/smileGenerator.cpp \
	$$PWD/simpleGenerators/trikEnginesGenerator.cpp \
	$$PWD/simpleGenerators/trikEnginesStopGenerator.cpp \

include(robotCommunication/robotCommunication.pri)
