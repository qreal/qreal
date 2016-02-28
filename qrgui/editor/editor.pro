# Copyright 2007-2016 QReal Research Group
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

TARGET = qrgui-editor

include(../../global.pri)

TEMPLATE = lib

links(qrkernel qslog qrutils \
		qrgui-mouse-gestures qrgui-thirdparty qrgui-models qrgui-meta-meta-model \
		qrgui-controller qrgui-plugin-manager qrgui-brand-manager \
		qrgui-dialogs \
)

includes(qrgraph qrgui qrgui/plugins/metaMetaModel)

QT += widgets

DEFINES += QRGUI_EDITOR_LIBRARY

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_editor_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_editor_fr.ts \

HEADERS += \
	$$PWD/editorDeclSpec.h \
	$$PWD/editorView.h \
	$$PWD/editorViewScene.h \
	$$PWD/sceneCustomizationInterface.h \
	$$PWD/sceneCustomizer.h \
	$$PWD/propertyEditorView.h \
	$$PWD/private/pushButtonProperty.h \
	$$PWD/private/editorViewMVIface.h \
	$$PWD/private/exploserView.h \
	$$PWD/private/clipboardHandler.h \
	$$PWD/private/touchSupportManager.h \
	$$PWD/edgeElement.h \
	$$PWD/element.h \
	$$PWD/nodeElement.h \
	$$PWD/labels/label.h \
	$$PWD/labels/labelInterface.h \
	$$PWD/contextMenuAction.h \
	$$PWD/embedded/linkers/embeddedLinker.h \
	$$PWD/embedded/linkers/embeddedLinkers.h \
	$$PWD/private/sceneGridHandler.h \
	$$PWD/private/resizeHandler.h \
	$$PWD/private/umlPortHandler.h \
	$$PWD/private/borderChecker.h \
	$$PWD/private/portHandler.h \
	$$PWD/ports/portFactory.h \
	$$PWD/ports/statLine.h \
	$$PWD/ports/statPoint.h \
	$$PWD/private/lineHandler.h \
	$$PWD/private/squareLine.h \
	$$PWD/private/brokenLine.h \
	$$PWD/private/curveLine.h \
	$$PWD/private/lineFactory.h \
	$$PWD/private/edgeArrangeCriteria.h \
	$$PWD/commands/elementCommand.h \
	$$PWD/commands/nodeElementCommand.h \
	$$PWD/commands/edgeElementCommand.h \
	$$PWD/commands/arrangeLinksCommand.h \
	$$PWD/commands/selectElementCommand.h \
	$$PWD/commands/updateElementCommand.h \
	$$PWD/commands/insertIntoEdgeCommand.h \
	$$PWD/commands/createAndUpdatePatternCommand.h \
	$$PWD/commands/removeAndUpdateCommand.h \
	$$PWD/commands/resizeCommand.h \
	$$PWD/commands/foldCommand.h \
	$$PWD/commands/reshapeEdgeCommand.h \
	$$PWD/commands/expandCommand.h \

SOURCES += \
	$$PWD/editorView.cpp \
	$$PWD/editorViewScene.cpp \
	$$PWD/sceneCustomizer.cpp \
	$$PWD/propertyEditorView.cpp \
	$$PWD/private/pushButtonProperty.cpp \
	$$PWD/private/editorViewMVIface.cpp \
	$$PWD/private/exploserView.cpp \
	$$PWD/private/clipboardHandler.cpp \
	$$PWD/private/touchSupportManager.cpp \
	$$PWD/edgeElement.cpp \
	$$PWD/element.cpp \
	$$PWD/nodeElement.cpp \
	$$PWD/labels/label.cpp \
	$$PWD/contextMenuAction.cpp \
	$$PWD/embedded/linkers/embeddedLinker.cpp \
	$$PWD/embedded/linkers/embeddedLinkers.cpp \
	$$PWD/private/sceneGridHandler.cpp \
	$$PWD/private/resizeHandler.cpp \
	$$PWD/private/umlPortHandler.cpp \
	$$PWD/private/borderChecker.cpp \
	$$PWD/private/portHandler.cpp \
	$$PWD/ports/portFactory.cpp \
	$$PWD/ports/statLine.cpp \
	$$PWD/ports/statPoint.cpp \
	$$PWD/private/lineHandler.cpp \
	$$PWD/private/squareLine.cpp \
	$$PWD/private/brokenLine.cpp \
	$$PWD/private/curveLine.cpp \
	$$PWD/private/lineFactory.cpp \
	$$PWD/private/edgeArrangeCriteria.cpp \
	$$PWD/commands/elementCommand.cpp \
	$$PWD/commands/nodeElementCommand.cpp \
	$$PWD/commands/edgeElementCommand.cpp \
	$$PWD/commands/arrangeLinksCommand.cpp \
	$$PWD/commands/selectElementCommand.cpp \
	$$PWD/commands/updateElementCommand.cpp \
	$$PWD/commands/insertIntoEdgeCommand.cpp \
	$$PWD/commands/createAndUpdatePatternCommand.cpp \
	$$PWD/commands/removeAndUpdateCommand.cpp \
	$$PWD/commands/resizeCommand.cpp \
	$$PWD/commands/foldCommand.cpp \
	$$PWD/commands/reshapeEdgeCommand.cpp \
	$$PWD/commands/expandCommand.cpp \

RESOURCES += \
	$$PWD/contextIcons.qrc \
