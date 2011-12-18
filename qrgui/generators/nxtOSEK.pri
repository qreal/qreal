# nxtOSEK sequential generator
HEADERS += generators/nxtOSEK/nxtOSEKgenerator.h \
		generators/nxtOSEK/sequentialGenerator.h \
		generators/nxtOSEK/smartLine.h
SOURCES += generators/nxtOSEK/sequentialGenerator/sequentialGenerator.cpp \
		generators/nxtOSEK/private/smartLine.cpp

# nxtOSEK function oriented generator

#generators/nxtOSEK/funcOrientedGenerator/elementGenerators/
HEADERS += generators/nxtOSEK/funcOrientedGenerator.h
HEADERS += generators/nxtOSEK/funcOrientedGenerator/abstractElementGenerator.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGeneratorFactory.h

SOURCES += generators/nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGeneratorFactory.cpp \
	generators/nxtOSEK/funcOrientedGenerator/abstractElementGenerator.cpp

#simpleElements/
HEADERS += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElementGenerator.h
SOURCES += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElementGenerator.cpp

HEADERS += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/beep_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesBackward_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesForward_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesStop_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/finalNode_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialBlock_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialNode_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/nullificationEncoder_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/playTone_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/timer_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColor_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorIntensity_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForEncoder_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForSonarDistance_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForTouchSensor_gen.h \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/function_gen.h

SOURCES += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/beep_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesBackward_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesForward_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/enginesStop_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/finalNode_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialBlock_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/initialNode_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/nullificationEncoder_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/playTone_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/timer_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColor_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForColorIntensity_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForEncoder_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForSonarDistance_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/waitForTouchSensor_gen.cpp \
	generators/nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElements/function_gen.cpp

HEADERS += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.h
SOURCES += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.cpp

HEADERS += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.h
SOURCES += generators/nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.cpp

