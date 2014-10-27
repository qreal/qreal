QT += widgets network

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-trik-qts-generator

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
		-lrobots-generator-base -lrobots-interpreter-base \

INCLUDEPATH += \
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

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikGenerator_ru.ts

RESOURCES = \
	$$PWD/trikQtsGenerator.qrc \
	$$PWD/templates.qrc \

HEADERS += \
	$$PWD/trikQtsGeneratorPlugin.h \
	$$PWD/trikQtsMasterGenerator.h \
	$$PWD/trikQtsGeneratorCustomizer.h \
	$$PWD/trikQtsGeneratorFactory.h \
	$$PWD/parts/trikDeviceVariables.h \
	$$PWD/parts/trikVariables.h \
	$$PWD/converters/backgroundColorConverter.h \
	$$PWD/converters/ledColorConverter.h \
	$$PWD/converters/trikStringPropertyConverter.h \
	$$PWD/simpleGenerators/detectGenerator.h \
	$$PWD/simpleGenerators/initCameraGenerator.h \
	$$PWD/simpleGenerators/ledGenerator.h \
	$$PWD/simpleGenerators/detectorToVariableGenerator.h \
	$$PWD/simpleGenerators/playToneGenerator.h \
	$$PWD/simpleGenerators/sadSmileGenerator.h \
	$$PWD/simpleGenerators/sayGenerator.h \
	$$PWD/simpleGenerators/systemGenerator.h \
	$$PWD/simpleGenerators/setBackgroundGenerator.h \
	$$PWD/simpleGenerators/smileGenerator.h \
	$$PWD/simpleGenerators/trikEnginesGenerator.h \
	$$PWD/simpleGenerators/waitForInfraredSensorGenerator.h \
	$$PWD/simpleGenerators/waitForMotionGenerator.h \
	$$PWD/simpleGenerators/drawLineGenerator.h \
	$$PWD/simpleGenerators/drawPixelGenerator.h \
	$$PWD/simpleGenerators/drawRectGenerator.h \
	$$PWD/simpleGenerators/drawEllipseGenerator.h \
	$$PWD/simpleGenerators/drawArcGenerator.h \
	$$PWD/simpleGenerators/setPainterWidthGenerator.h \
	$$PWD/simpleGenerators/setPainterColorGenerator.h \
	$$PWD/simpleGenerators/sendMessageGenerator.h \
	$$PWD/simpleGenerators/waitForMessageGenerator.h \

SOURCES += \
	$$PWD/trikQtsGeneratorPlugin.cpp \
	$$PWD/trikQtsMasterGenerator.cpp \
	$$PWD/trikQtsGeneratorCustomizer.cpp \
	$$PWD/trikQtsGeneratorFactory.cpp \
	$$PWD/parts/trikDeviceVariables.cpp \
	$$PWD/parts/trikVariables.cpp \
	$$PWD/converters/backgroundColorConverter.cpp \
	$$PWD/converters/ledColorConverter.cpp \
	$$PWD/converters/trikStringPropertyConverter.cpp \
	$$PWD/simpleGenerators/detectGenerator.cpp \
	$$PWD/simpleGenerators/initCameraGenerator.cpp \
	$$PWD/simpleGenerators/ledGenerator.cpp \
	$$PWD/simpleGenerators/detectorToVariableGenerator.cpp \
	$$PWD/simpleGenerators/playToneGenerator.cpp \
	$$PWD/simpleGenerators/sadSmileGenerator.cpp \
	$$PWD/simpleGenerators/sayGenerator.cpp \
	$$PWD/simpleGenerators/systemGenerator.cpp \
	$$PWD/simpleGenerators/setBackgroundGenerator.cpp \
	$$PWD/simpleGenerators/smileGenerator.cpp \
	$$PWD/simpleGenerators/trikEnginesGenerator.cpp \
	$$PWD/simpleGenerators/waitForInfraredSensorGenerator.cpp \
	$$PWD/simpleGenerators/waitForMotionGenerator.cpp \
	$$PWD/simpleGenerators/drawLineGenerator.cpp \
	$$PWD/simpleGenerators/drawPixelGenerator.cpp \
	$$PWD/simpleGenerators/drawRectGenerator.cpp \
	$$PWD/simpleGenerators/drawEllipseGenerator.cpp \
	$$PWD/simpleGenerators/drawArcGenerator.cpp \
	$$PWD/simpleGenerators/setPainterWidthGenerator.cpp \
	$$PWD/simpleGenerators/setPainterColorGenerator.cpp \
	$$PWD/simpleGenerators/sendMessageGenerator.cpp \
	$$PWD/simpleGenerators/waitForMessageGenerator.cpp \

include(robotCommunication/robotCommunication.pri)
