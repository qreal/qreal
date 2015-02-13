TARGET = qrgui-controller

include(../../global.pri)

TEMPLATE = lib

links(qrkernel)
includes(qrgui)

QT += widgets

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
