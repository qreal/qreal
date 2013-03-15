HEADERS += \
	controller/controller.h \
	controller/commands/abstractCommand.h \
	controller/commands/doNothingCommand.h \
	controller/commands/createElementCommand.h \
	controller/commands/createRemoveCommandImplementation.h \
	controller/commands/removeElementCommand.h \
    controller/commands/changePropertyCommand.h \
    controller/undoStack.h

SOURCES += \
	controller/controller.cpp \
	controller/commands/abstractCommand.cpp \
	controller/commands/doNothingCommand.cpp \
	controller/commands/createElementCommand.cpp \
	controller/commands/createRemoveCommandImplementation.cpp \
	controller/commands/removeElementCommand.cpp \
    controller/commands/changePropertyCommand.cpp \
    controller/undoStack.cpp
