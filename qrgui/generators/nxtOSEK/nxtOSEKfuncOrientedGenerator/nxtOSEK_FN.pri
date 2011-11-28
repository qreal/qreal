#generators/nxtOSEK_FN/elementGenerators/
HEADERS += generators/nxtOSEK_FN/nxtOSEKRobotGenerator.h \
	generators/nxtOSEK_FN/smartLine.h \
	generators/nxtOSEK_FN/abstractElementGenerator.h \
	generators/nxtOSEK_FN/elementGeneratorFactory.h

SOURCES += generators/nxtOSEK_FN/nxtOSEKRobotGenerator.cpp \
	generators/nxtOSEK_FN/smartLine.cpp \
	generators/nxtOSEK_FN/elementGeneratorFactory.cpp \
	generators/nxtOSEK_FN/abstractElementGenerator.cpp

#simpleElements/
HEADERS += generators/nxtOSEK_FN/elementGenerators/simpleElementGenerator.h
SOURCES += generators/nxtOSEK_FN/elementGenerators/simpleElementGenerator.cpp

HEADERS += generators/nxtOSEK_FN/elementGenerators/simpleElements/beep_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/enginesBackward_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/enginesForward_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/enginesStop_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/finalNode_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/initialBlock_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/initialNode_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/nullificationEncoder_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/playTone_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/timer_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForColor_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForColorIntensity_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForEncoder_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForSonarDistance_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForTouchSensor_gen.h \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/function_gen.h

SOURCES += generators/nxtOSEK_FN/elementGenerators/simpleElements/beep_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/enginesBackward_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/enginesForward_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/enginesStop_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/finalNode_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/initialBlock_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/initialNode_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/nullificationEncoder_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/playTone_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/timer_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForColor_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForColorIntensity_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForEncoder_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForSonarDistance_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/waitForTouchSensor_gen.cpp \
	generators/nxtOSEK_FN/elementGenerators/simpleElements/function_gen.cpp

HEADERS += generators/nxtOSEK_FN/elementGenerators/loop_gen.h
SOURCES += generators/nxtOSEK_FN/elementGenerators/loop_gen.cpp

HEADERS += generators/nxtOSEK_FN/elementGenerators/ifBlock_gen.h
SOURCES += generators/nxtOSEK_FN/elementGenerators/ifBlock_gen.cpp
