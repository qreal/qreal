# Visual debugger
HEADERS += \
	interpreters/visualDebugger/visualDebugger.h \
	interpreters/visualDebugger/blockParser.h \
	interpreters/visualDebugger/number.h \
	interpreters/visualDebugger/debuggerConnector.h \

SOURCES += \
	interpreters/visualDebugger/visualDebugger.cpp \
	interpreters/visualDebugger/blockParser.cpp \
	interpreters/visualDebugger/number.cpp \
	interpreters/visualDebugger/debuggerConnector.cpp \

# Files for parsiong of expressions on diagrams
HEADERS += \
	interpreters/expressionsParser/expressionsParser.h \

SOURCES += \
	interpreters/expressionsParser/expressionsParser.cpp \
