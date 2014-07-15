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
	$$PWD/converters/backgroundColorConverter.h \
	$$PWD/converters/ledColorConverter.h \
	$$PWD/converters/engineV4PortConverter.h \
	$$PWD/converters/engineV6PortConverter.h \
	$$PWD/converters/encoderV4PortConverter.h \
	$$PWD/converters/encoderV6PortConverter.h \
	$$PWD/converters/trikStringPropertyConverter.h \
	$$PWD/simpleGenerators/detectLineGenerator.h \
	$$PWD/simpleGenerators/initCameraGenerator.h \
	$$PWD/simpleGenerators/ledGenerator.h \
	$$PWD/simpleGenerators/lineDetectorToVariableGenerator.h \
	$$PWD/simpleGenerators/playToneGenerator.h \
	$$PWD/simpleGenerators/sadSmileGenerator.h \
	$$PWD/simpleGenerators/sayGenerator.h \
	$$PWD/simpleGenerators/systemGenerator.h \
	$$PWD/simpleGenerators/setBackgroundGenerator.h \
	$$PWD/simpleGenerators/smileGenerator.h \
	$$PWD/simpleGenerators/trikEnginesGenerator.h \
	$$PWD/simpleGenerators/trikEnginesStopGenerator.h \
	$$PWD/simpleGenerators/trikNullificationEncoderGenerator.h \
	$$PWD/simpleGenerators/waitForInfraredSensorGenerator.h \
	$$PWD/simpleGenerators/waitForMotionGenerator.h \
	$$PWD/simpleGenerators/drawLineGenerator.h \
	$$PWD/simpleGenerators/drawPixelGenerator.h \
	$$PWD/simpleGenerators/drawRectGenerator.h \
	$$PWD/simpleGenerators/drawEllipseGenerator.h \
	$$PWD/simpleGenerators/drawArcGenerator.h \
	$$PWD/simpleGenerators/setPainterWidthGenerator.h \
	$$PWD/simpleGenerators/setPainterColorGenerator.h

SOURCES += \
	$$PWD/trikGeneratorPlugin.cpp \
	$$PWD/trikMasterGenerator.cpp \
	$$PWD/trikGeneratorCustomizer.cpp \
	$$PWD/trikGeneratorFactory.cpp \
	$$PWD/parts/trikVariables.cpp \
	$$PWD/converters/backgroundColorConverter.cpp \
	$$PWD/converters/ledColorConverter.cpp \
	$$PWD/converters/engineV4PortConverter.cpp \
	$$PWD/converters/engineV6PortConverter.cpp \
	$$PWD/converters/encoderV4PortConverter.cpp \
	$$PWD/converters/encoderV6PortConverter.cpp \
	$$PWD/converters/trikStringPropertyConverter.cpp \
	$$PWD/simpleGenerators/detectLineGenerator.cpp \
	$$PWD/simpleGenerators/initCameraGenerator.cpp \
	$$PWD/simpleGenerators/ledGenerator.cpp \
	$$PWD/simpleGenerators/lineDetectorToVariableGenerator.cpp \
	$$PWD/simpleGenerators/playToneGenerator.cpp \
	$$PWD/simpleGenerators/sadSmileGenerator.cpp \
	$$PWD/simpleGenerators/sayGenerator.cpp \
	$$PWD/simpleGenerators/systemGenerator.cpp \
	$$PWD/simpleGenerators/setBackgroundGenerator.cpp \
	$$PWD/simpleGenerators/smileGenerator.cpp \
	$$PWD/simpleGenerators/trikEnginesGenerator.cpp \
	$$PWD/simpleGenerators/trikEnginesStopGenerator.cpp \
	$$PWD/simpleGenerators/trikNullificationEncoderGenerator.cpp \
	$$PWD/simpleGenerators/waitForInfraredSensorGenerator.cpp \
	$$PWD/simpleGenerators/waitForMotionGenerator.cpp \
	$$PWD/simpleGenerators/drawLineGenerator.cpp \
	$$PWD/simpleGenerators/drawPixelGenerator.cpp \
	$$PWD/simpleGenerators/drawRectGenerator.cpp \
	$$PWD/simpleGenerators/drawEllipseGenerator.cpp \
	$$PWD/simpleGenerators/drawArcGenerator.cpp \
	$$PWD/simpleGenerators/setPainterWidthGenerator.cpp \
	$$PWD/simpleGenerators/setPainterColorGenerator.cpp

include(robotCommunication/robotCommunication.pri)
