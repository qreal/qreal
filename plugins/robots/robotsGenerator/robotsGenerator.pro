QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGenerator_ru.ts

RESOURCES = robotsGenerator.qrc

HEADERS += \
	robotsGeneratorPlugin.h \
	nxtOSEK/smartLine.h \
	nxtOSEK/nxtFlashTool.h \
    nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlockGenerator.h

SOURCES += \
	robotsGeneratorPlugin.cpp \
	nxtOSEK/smartLine.cpp \
	nxtOSEK/nxtFlashTool.cpp

# nxtOSEK base generator
HEADERS += nxtOSEK/nxtOSEKgenerator.h
SOURCES += nxtOSEK/nxtOSEKgenerator.cpp

# nxtOSEK sequential generator
HEADERS += nxtOSEK/sequentialGenerator.h
SOURCES += nxtOSEK/sequentialGenerator/sequentialGenerator.cpp

# nxtOSEK function oriented generator

#nxtOSEK/funcOrientedGenerator/elementGenerators/
HEADERS += nxtOSEK/funcOrientedGenerator.h
HEADERS += \
	nxtOSEK/funcOrientedGenerator/abstractElementGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGeneratorFactory.h

SOURCES += \
	nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGeneratorFactory.cpp \
	nxtOSEK/funcOrientedGenerator/abstractElementGenerator.cpp

#simpleElements/
HEADERS += nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElementGenerator.h
SOURCES += nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElementGenerator.cpp

HEADERS += \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/beepGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesBackwardGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesForwardGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesStopGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/finalNodeGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialBlockGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialNodeGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/nullificationEncoderGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/playToneGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/timerGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorIntensityGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForEncoderGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForSonarDistanceGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForTouchSensorGenerator.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/functionGenerator.h

SOURCES += \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/beepGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesBackwardGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesForwardGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesStopGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/finalNodeGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialBlockGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialNodeGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/nullificationEncoderGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/playToneGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/timerGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorIntensityGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForEncoderGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForSonarDistanceGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForTouchSensorGenerator.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/functionGenerator.cpp

HEADERS += nxtOSEK/funcOrientedGenerator/elementGenerators/loopGenerator.h
SOURCES += nxtOSEK/funcOrientedGenerator/elementGenerators/loopGenerator.cpp

HEADERS +=
SOURCES += nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlockGenerator.cpp

