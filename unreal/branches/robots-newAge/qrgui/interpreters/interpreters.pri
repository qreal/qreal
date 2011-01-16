# Visual debugger
HEADERS += \
	interpreters/visualDebugger/visualDebugger.h \
	interpreters/visualDebugger/blockParser.h \
	interpreters/visualDebugger/number.h

SOURCES += \
	interpreters/visualDebugger/visualDebugger.cpp \
	interpreters/visualDebugger/blockParser.cpp \
	interpreters/visualDebugger/number.cpp

# Robots behavior diagrams interpreter
HEADERS += \
	interpreters/robots/interpreter.h \
	interpreters/robots/robotCommunicationInterface.h \
	interpreters/robots/bluetoothRobotCommunication.h \
	interpreters/robots/details/thread.h \

SOURCES += \
	interpreters/robots/interpreter.cpp \
	interpreters/robots/bluetoothRobotCommunication.cpp \
	interpreters/robots/details/thread.cpp \

