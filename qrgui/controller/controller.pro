TEMPLATE = lib

DESTDIR = $$PWD/../../bin

LIBS += -L$$PWD/../../bin -lqrkernel

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

DEFINES += QRGUI_CONTROLLER_LIBRARY

HEADERS += \
	$$PWD/controllerDeclSpec.h \
	$$PWD/controller.h \
	$$PWD/undoStack.h \
	$$PWD/commands/abstractCommand.h \
	$$PWD/commands/doNothingCommand.h \
	$$PWD/commands/trackingEntity.h \

SOURCES += \
	$$PWD/controller.cpp \
	$$PWD/undoStack.cpp \
	$$PWD/commands/abstractCommand.cpp \
	$$PWD/commands/doNothingCommand.cpp \
	$$PWD/commands/trackingEntity.cpp \
