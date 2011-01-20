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
	interpreters/robots/details/blocksFactory.h \
	interpreters/robots/details/blocksTable.h \
	interpreters/robots/details/robotModel.h \
	interpreters/robots/details/bluetoothRobotCommunicationThread.h \
	interpreters/robots/details/blocks/block.h \
	interpreters/robots/details/blocks/dummyBlock.h \
	interpreters/robots/details/blocks/timerBlock.h \
	interpreters/robots/details/blocks/beepBlock.h \
	interpreters/robots/details/blocks/initialBlock.h \
	interpreters/robots/details/robotParts/brick.h \
	interpreters/robots/details/robotParts/robotPart.h \

SOURCES += \
	interpreters/robots/interpreter.cpp \
	interpreters/robots/bluetoothRobotCommunication.cpp \
	interpreters/robots/details/thread.cpp \
	interpreters/robots/details/blocksTable.cpp \
	interpreters/robots/details/blocksFactory.cpp \
	interpreters/robots/details/robotModel.cpp \
	interpreters/robots/details/bluetoothRobotCommunicationThread.cpp \
	interpreters/robots/details/blocks/block.cpp \
	interpreters/robots/details/blocks/dummyBlock.cpp \
	interpreters/robots/details/blocks/timerBlock.cpp \
	interpreters/robots/details/blocks/beepBlock.cpp \
	interpreters/robots/details/blocks/initialBlock.cpp \
	interpreters/robots/details/robotParts/brick.cpp \
	interpreters/robots/details/robotParts/robotPart.cpp \
