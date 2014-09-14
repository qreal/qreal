TEMPLATE = lib

DESTDIR = $$PWD/../../bin

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

QT += widgets

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS += \
	$$PWD/controller.h \
	$$PWD/undoStack.h \
	$$PWD/commands/abstractCommand.h \
	$$PWD/commands/doNothingCommand.h \
	$$PWD/commands/createElementCommand.h \
	$$PWD/commands/createRemoveCommandImplementation.h \
	$$PWD/commands/removeElementCommand.h \
	$$PWD/commands/changePropertyCommand.h \
	$$PWD/commands/elementCommand.h \
	$$PWD/commands/nodeElementCommand.h \
	$$PWD/commands/edgeElementCommand.h \
	$$PWD/commands/arrangeLinksCommand.h \
	$$PWD/commands/selectElementCommand.h \
	$$PWD/commands/updateElementCommand.h \
	$$PWD/commands/changeParentCommand.h \
	$$PWD/commands/trackingEntity.h \
	$$PWD/commands/createGroupCommand.h \
	$$PWD/commands/insertIntoEdgeCommand.h \
	$$PWD/commands/renameCommand.h \
	$$PWD/commands/explosionCommand.h \
	$$PWD/commands/renameExplosionCommand.h \

SOURCES += \
	$$PWD/controller.cpp \
	$$PWD/undoStack.cpp \
	$$PWD/commands/abstractCommand.cpp \
	$$PWD/commands/doNothingCommand.cpp \
	$$PWD/commands/createElementCommand.cpp \
	$$PWD/commands/createRemoveCommandImplementation.cpp \
	$$PWD/commands/removeElementCommand.cpp \
	$$PWD/commands/changePropertyCommand.cpp \
	$$PWD/commands/elementCommand.cpp \
	$$PWD/commands/nodeElementCommand.cpp \
	$$PWD/commands/edgeElementCommand.cpp \
	$$PWD/commands/arrangeLinksCommand.cpp \
	$$PWD/commands/selectElementCommand.cpp \
	$$PWD/commands/updateElementCommand.cpp \
	$$PWD/commands/changeParentCommand.cpp \
	$$PWD/commands/trackingEntity.cpp \
	$$PWD/commands/createGroupCommand.cpp \
	$$PWD/commands/insertIntoEdgeCommand.cpp \
	$$PWD/commands/renameCommand.cpp \
	$$PWD/commands/explosionCommand.cpp \
	$$PWD/commands/renameExplosionCommand.cpp \
