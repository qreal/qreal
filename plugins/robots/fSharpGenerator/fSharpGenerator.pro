QT += widgets network

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
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

TRANSLATIONS = fSharpGenerator_ru.ts

RESOURCES = \
	$$PWD/templates.qrc \

HEADERS += \
        $$PWD/fSharpGeneratorPlugin.h \
        $$PWD/fSharpMasterGenerator.h \
        $$PWD/fSharpGeneratorCustomizer.h \
        $$PWD/fSharpGeneratorFactory.h \
        $$PWD/parts/fSharpVariables.h \
        $$PWD/converters/backgroundColorConverter.h \
        $$PWD/converters/ledColorConverter.h \
        $$PWD/converters/powerMotorPortConverter.h \
        $$PWD/converters/servoMotorPortConverter.h \
        $$PWD/converters/functionFSharpBlockConverter.h \
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
        $$PWD/simpleGenerators/fSharpEnginesGenerator.h \
        $$PWD/simpleGenerators/fSharpEnginesStopGenerator.h \
        $$PWD/simpleGenerators/waitForButtonGenerator.h \

SOURCES += \
        $$PWD/fSharpGeneratorPlugin.cpp \
        $$PWD/fSharpMasterGenerator.cpp \
        $$PWD/fSharpGeneratorCustomizer.cpp \
        $$PWD/fSharpGeneratorFactory.cpp \
        $$PWD/parts/fSharpVariables.cpp \
        $$PWD/converters/backgroundColorConverter.cpp \
        $$PWD/converters/ledColorConverter.cpp \
        $$PWD/converters/powerMotorPortConverter.cpp \
        $$PWD/converters/servoMotorPortConverter.cpp \
        $$PWD/converters/functionFSharpBlockConverter.cpp \
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
        $$PWD/simpleGenerators/fSharpEnginesGenerator.cpp \
        $$PWD/simpleGenerators/fSharpEnginesStopGenerator.cpp \
        $$PWD/simpleGenerators/waitForButtonGenerator.cpp \

