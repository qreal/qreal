TEMPLATE = lib

DESTDIR = $$PWD/../../bin

QT += widgets

LIBS += -L$$PWD/../../bin -lqrkernel -lqrrepo -lpluginManager -lcontroller

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS += \
	$$PWD/models.h \
	$$PWD/graphicalModelAssistApi.h \
	$$PWD/logicalModelAssistApi.h \
	$$PWD/exploser.h \
	$$PWD/commands/createElementCommand.h \
	$$PWD/commands/removeElementCommand.h \
	$$PWD/commands/createRemoveCommandImplementation.h \
	$$PWD/commands/changePropertyCommand.h \
	$$PWD/commands/changeParentCommand.h \
	$$PWD/commands/renameCommand.h \
	$$PWD/commands/explosionCommand.h \
	$$PWD/commands/renameExplosionCommand.h \
	$$PWD/details/graphicalModel.h \
	$$PWD/details/graphicalPartModel.h \
	$$PWD/details/graphicalModelView.h \
	$$PWD/details/logicalModel.h \
	$$PWD/details/logicalModelView.h \
	$$PWD/details/modelsAssistApi.h \
	$$PWD/details/modelsImplementation/graphicalModelItem.h \
	$$PWD/details/modelsImplementation/abstractModel.h \
	$$PWD/details/modelsImplementation/abstractModelItem.h \
	$$PWD/details/modelsImplementation/graphicalPartModelItem.h \
	$$PWD/details/modelsImplementation/logicalModelItem.h \
	$$PWD/details/modelsImplementation/abstractView.h \
	$$PWD/details/modelsImplementation/modelIndexesInterface.h \

SOURCES += \
	$$PWD/models.cpp \
	$$PWD/graphicalModelAssistApi.cpp \
	$$PWD/logicalModelAssistApi.cpp \
	$$PWD/exploser.cpp \
	$$PWD/commands/createElementCommand.cpp \
	$$PWD/commands/removeElementCommand.cpp \
	$$PWD/commands/createRemoveCommandImplementation.cpp \
	$$PWD/commands/changePropertyCommand.cpp \
	$$PWD/commands/changeParentCommand.cpp \
	$$PWD/commands/renameCommand.cpp \
	$$PWD/commands/explosionCommand.cpp \
	$$PWD/commands/renameExplosionCommand.cpp \
	$$PWD/details/graphicalModel.cpp \
	$$PWD/details/graphicalPartModel.cpp \
	$$PWD/details/graphicalModelView.cpp \
	$$PWD/details/logicalModel.cpp \
	$$PWD/details/logicalModelView.cpp \
	$$PWD/details/modelsAssistApi.cpp \
	$$PWD/details/modelsImplementation/graphicalModelItem.cpp \
	$$PWD/details/modelsImplementation/abstractModel.cpp \
	$$PWD/details/modelsImplementation/abstractModelItem.cpp \
	$$PWD/details/modelsImplementation/graphicalPartModelItem.cpp \
	$$PWD/details/modelsImplementation/logicalModelItem.cpp \
	$$PWD/details/modelsImplementation/abstractView.cpp \
