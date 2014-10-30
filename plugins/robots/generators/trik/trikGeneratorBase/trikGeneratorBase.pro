QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/
TARGET = robots-trik-generator-base

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
		-lrobots-generator-base -lrobots-interpreter-base \

DEFINES += ROBOTS_TRIK_GENERATOR_BASE_LIBRARY

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

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikGeneratorBase_ru.ts

HEADERS += \
	$$PWD/include/trikGeneratorBase/trikGeneratorBaseDeclSpec.h \
	$$PWD/include/trikGeneratorBase/trikGeneratorPluginBase.h \
	$$PWD/include/trikGeneratorBase/trikMasterGeneratorBase.h \
	$$PWD/src/trikGeneratorCustomizer.h \
	$$PWD/src/trikGeneratorFactory.h \
	$$PWD/src/parts/trikDeviceVariables.h \
	$$PWD/src/converters/backgroundColorConverter.h \
	$$PWD/src/converters/ledColorConverter.h \
	$$PWD/src/converters/trikStringPropertyConverter.h \
	$$PWD/src/simpleGenerators/detectGenerator.h \
	$$PWD/src/simpleGenerators/initCameraGenerator.h \
	$$PWD/src/simpleGenerators/ledGenerator.h \
	$$PWD/src/simpleGenerators/detectorToVariableGenerator.h \
	$$PWD/src/simpleGenerators/playToneGenerator.h \
	$$PWD/src/simpleGenerators/sadSmileGenerator.h \
	$$PWD/src/simpleGenerators/sayGenerator.h \
	$$PWD/src/simpleGenerators/systemGenerator.h \
	$$PWD/src/simpleGenerators/setBackgroundGenerator.h \
	$$PWD/src/simpleGenerators/smileGenerator.h \
	$$PWD/src/simpleGenerators/trikEnginesGenerator.h \
	$$PWD/src/simpleGenerators/waitForInfraredSensorGenerator.h \
	$$PWD/src/simpleGenerators/waitForMotionGenerator.h \
	$$PWD/src/simpleGenerators/drawLineGenerator.h \
	$$PWD/src/simpleGenerators/drawPixelGenerator.h \
	$$PWD/src/simpleGenerators/drawRectGenerator.h \
	$$PWD/src/simpleGenerators/drawEllipseGenerator.h \
	$$PWD/src/simpleGenerators/drawArcGenerator.h \
	$$PWD/src/simpleGenerators/setPainterWidthGenerator.h \
	$$PWD/src/simpleGenerators/setPainterColorGenerator.h \
	$$PWD/src/simpleGenerators/sendMessageGenerator.h \
	$$PWD/src/simpleGenerators/waitForMessageGenerator.h \

SOURCES += \
	$$PWD/src/trikGeneratorPluginBase.cpp \
	$$PWD/src/trikMasterGeneratorBase.cpp \
	$$PWD/src/trikGeneratorCustomizer.cpp \
	$$PWD/src/trikGeneratorFactory.cpp \
	$$PWD/src/parts/trikDeviceVariables.cpp \
	$$PWD/src/converters/backgroundColorConverter.cpp \
	$$PWD/src/converters/ledColorConverter.cpp \
	$$PWD/src/converters/trikStringPropertyConverter.cpp \
	$$PWD/src/simpleGenerators/detectGenerator.cpp \
	$$PWD/src/simpleGenerators/initCameraGenerator.cpp \
	$$PWD/src/simpleGenerators/ledGenerator.cpp \
	$$PWD/src/simpleGenerators/detectorToVariableGenerator.cpp \
	$$PWD/src/simpleGenerators/playToneGenerator.cpp \
	$$PWD/src/simpleGenerators/sadSmileGenerator.cpp \
	$$PWD/src/simpleGenerators/sayGenerator.cpp \
	$$PWD/src/simpleGenerators/systemGenerator.cpp \
	$$PWD/src/simpleGenerators/setBackgroundGenerator.cpp \
	$$PWD/src/simpleGenerators/smileGenerator.cpp \
	$$PWD/src/simpleGenerators/trikEnginesGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForInfraredSensorGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForMotionGenerator.cpp \
	$$PWD/src/simpleGenerators/drawLineGenerator.cpp \
	$$PWD/src/simpleGenerators/drawPixelGenerator.cpp \
	$$PWD/src/simpleGenerators/drawRectGenerator.cpp \
	$$PWD/src/simpleGenerators/drawEllipseGenerator.cpp \
	$$PWD/src/simpleGenerators/drawArcGenerator.cpp \
	$$PWD/src/simpleGenerators/setPainterWidthGenerator.cpp \
	$$PWD/src/simpleGenerators/setPainterColorGenerator.cpp \
	$$PWD/src/simpleGenerators/sendMessageGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForMessageGenerator.cpp \
