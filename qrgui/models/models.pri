# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

QT += widgets

links(qrkernel qrrepo qrgui-plugin-manager qrgui-controller)
includes(qrgui qrgui/plugins/metaMetaModel)

HEADERS += \
	$$PWD/modelsDeclSpec.h \
	$$PWD/models.h \
	$$PWD/graphicalModelAssistApi.h \
	$$PWD/logicalModelAssistApi.h \
	$$PWD/exploser.h \
	$$PWD/propertyEditorModel.h \
	$$PWD/elementInfo.h \
	$$PWD/nodeInfo.h \
	$$PWD/edgeInfo.h \
	$$PWD/commands/createElementsCommand.h \
	$$PWD/commands/createPatternCommand.h \
	$$PWD/commands/removeElementsCommand.h \
	$$PWD/commands/createRemoveCommandImplementation.h \
	$$PWD/commands/pasteCommand.h \
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
	$$PWD/elementInfo.cpp \
	$$PWD/nodeInfo.cpp \
	$$PWD/edgeInfo.cpp \
	$$PWD/commands/createElementsCommand.cpp \
	$$PWD/commands/removeElementsCommand.cpp \
	$$PWD/commands/createPatternCommand.cpp \
	$$PWD/commands/createRemoveCommandImplementation.cpp \
	$$PWD/commands/pasteCommand.cpp \
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
