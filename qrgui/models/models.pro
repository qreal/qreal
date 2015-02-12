TEMPLATE = lib

TARGET = qrgui-models

include(../../global.pri)

QT += widgets

links(qrkernel qrrepo qrgui-plugin-manager qrgui-controller)
includes(qrgui)

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_models_ru.ts

DEFINES += QRGUI_MODELS_LIBRARY

HEADERS += \
	$$PWD/modelsDeclSpec.h \
	$$PWD/models.h \
	$$PWD/graphicalModelAssistApi.h \
	$$PWD/logicalModelAssistApi.h \
	$$PWD/exploser.h \
	$$PWD/propertyEditorModel.h \
	$$PWD/commands/createElementCommand.h \
	$$PWD/commands/removeElementCommand.h \
	$$PWD/commands/multipleRemoveCommand.h \
	$$PWD/commands/createRemoveCommandImplementation.h \
	$$PWD/commands/createGroupCommand.h \
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
	$$PWD/details/renameDialog.h \
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
	$$PWD/propertyEditorModel.cpp \
	$$PWD/commands/createElementCommand.cpp \
	$$PWD/commands/removeElementCommand.cpp \
	$$PWD/commands/multipleRemoveCommand.cpp \
	$$PWD/commands/createRemoveCommandImplementation.cpp \
	$$PWD/commands/createGroupCommand.cpp \
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
	$$PWD/details/renameDialog.cpp \
	$$PWD/details/modelsImplementation/graphicalModelItem.cpp \
	$$PWD/details/modelsImplementation/abstractModel.cpp \
	$$PWD/details/modelsImplementation/abstractModelItem.cpp \
	$$PWD/details/modelsImplementation/graphicalPartModelItem.cpp \
	$$PWD/details/modelsImplementation/logicalModelItem.cpp \
	$$PWD/details/modelsImplementation/abstractView.cpp \
