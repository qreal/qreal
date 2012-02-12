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
	nxtOSEK/nxtFlashTool.h
#
#	nxtOSEK/nxtOSEKRobotGenerator.h \
#

SOURCES += \
	robotsGeneratorPlugin.cpp \
	nxtOSEK/smartLine.cpp \
	nxtOSEK/nxtFlashTool.cpp
#
#	nxtOSEK/nxtOSEKRobotGenerator.cpp \
#	

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
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/beep_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesBackward_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesForward_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesStop_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/finalNode_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialBlock_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialNode_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/nullificationEncoder_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/playTone_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/timer_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColor_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorIntensity_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForEncoder_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForSonarDistance_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForTouchSensor_gen.h \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/function_gen.h

SOURCES += \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/beep_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesBackward_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesForward_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesStop_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/finalNode_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialBlock_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialNode_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/nullificationEncoder_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/playTone_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/timer_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColor_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorIntensity_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForEncoder_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForSonarDistance_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForTouchSensor_gen.cpp \
	nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/function_gen.cpp

HEADERS += nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.h
SOURCES += nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.cpp

HEADERS += nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.h
SOURCES += nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.cpp

