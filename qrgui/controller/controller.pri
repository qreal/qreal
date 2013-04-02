HEADERS += \
	controller/controller.h \
	controller/undoStack.h \
	controller/commands/abstractCommand.h \
	controller/commands/doNothingCommand.h \
	controller/commands/createElementCommand.h \
	controller/commands/createRemoveCommandImplementation.h \
	controller/commands/removeElementCommand.h \
	controller/commands/changePropertyCommand.h \
	controller/commands/elementCommand.h \
	controller/commands/nodeElementCommand.h \
	controller/commands/edgeElementCommand.h \
	controller/commands/arrangeLinksCommand.h \
	controller/commands/selectElementCommand.h \
	controller/commands/updateElementCommand.h \
	controller/commands/changeParentCommand.h \
	controller/commands/trackingEntity.h \

SOURCES += \
	controller/controller.cpp \
	controller/undoStack.cpp \
	controller/commands/abstractCommand.cpp \
	controller/commands/doNothingCommand.cpp \
	controller/commands/createElementCommand.cpp \
	controller/commands/createRemoveCommandImplementation.cpp \
	controller/commands/removeElementCommand.cpp \
	controller/commands/changePropertyCommand.cpp \
	controller/commands/elementCommand.cpp \
	controller/commands/nodeElementCommand.cpp \
	controller/commands/edgeElementCommand.cpp \
	controller/commands/arrangeLinksCommand.cpp \
	controller/commands/selectElementCommand.cpp \
	controller/commands/updateElementCommand.cpp \
	controller/commands/changeParentCommand.cpp \
	controller/commands/trackingEntity.cpp \
