TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-editor

LIBS += -L$$PWD/../../bin -lqrkernel -lqslog -lqrutils \
		-lqrgui-mouse-gestures -lqrgui-thirdparty -lqrgui-models \
		-lqrgui-controller -lqrgui-plugin-manager -lqrgui-brand-manager \
		-lqrgui-dialogs \

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

DEFINES += QRGUI_EDITOR_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_editor_ru.ts

HEADERS += \
	$$PWD/editorDeclSpec.h \
	$$PWD/editorView.h \
	$$PWD/editorViewScene.h \
	$$PWD/sceneCustomizationInterface.h \
	$$PWD/sceneCustomizer.h \
	$$PWD/propertyEditorView.h \
	$$PWD/private/pushButtonProperty.h \
	$$PWD/private/editorViewMVIface.h \
	$$PWD/copyPaste/pasteCommand.h \
	$$PWD/copyPaste/clipboardHandler.h \
	$$PWD/copyPaste/pasteNodeCommand.h \
	$$PWD/copyPaste/pasteGroupCommand.h \
	$$PWD/copyPaste/pasteEdgeCommand.h \
	$$PWD/private/exploserView.h \
	$$PWD/private/touchSupportManager.h \
	$$PWD/edgeElement.h \
	$$PWD/element.h \
	$$PWD/nodeElement.h \
	$$PWD/label.h \
	$$PWD/labelFactory.h \
	$$PWD/contextMenuAction.h \
	$$PWD/embedded/linkers/embeddedLinker.h \
	$$PWD/embedded/linkers/embeddedLinkers.h \
	$$PWD/serializationData.h \
	$$PWD/private/sceneGridHandler.h \
	$$PWD/private/resizeHandler.h \
	$$PWD/private/umlPortHandler.h \
	$$PWD/private/borderChecker.h \
	$$PWD/private/portHandler.h \
	$$PWD/private/copyHandler.h \
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
	$$PWD/commands/createAndUpdateGroupCommand.h \
	$$PWD/commands/multipleRemoveAndUpdateCommand.h \
	$$PWD/commands/resizeCommand.h \
	$$PWD/commands/foldCommand.h \
	$$PWD/commands/reshapeEdgeCommand.h \
	$$PWD/commands/expandCommand.h \
	$$PWD/miniMap/miniMap.h \
	$$PWD/miniMap/miniMapButton.h \
	$$PWD/miniMap/miniMapShell.h

SOURCES += \
	$$PWD/editorView.cpp \
	$$PWD/editorViewScene.cpp \
	$$PWD/sceneCustomizer.cpp \
	$$PWD/propertyEditorView.cpp \
	$$PWD/private/pushButtonProperty.cpp \
	$$PWD/private/editorViewMVIface.cpp \
	$$PWD/copyPaste/pasteCommand.cpp \
	$$PWD/copyPaste/clipboardHandler.cpp \
	$$PWD/copyPaste/pasteNodeCommand.cpp \
	$$PWD/copyPaste/pasteGroupCommand.cpp \
	$$PWD/copyPaste/pasteEdgeCommand.cpp \
	$$PWD/private/exploserView.cpp \
	$$PWD/private/touchSupportManager.cpp \
	$$PWD/edgeElement.cpp \
	$$PWD/element.cpp \
	$$PWD/nodeElement.cpp \
	$$PWD/label.cpp \
	$$PWD/labelFactory.cpp \
	$$PWD/contextMenuAction.cpp \
	$$PWD/embedded/linkers/embeddedLinker.cpp \
	$$PWD/embedded/linkers/embeddedLinkers.cpp \
	$$PWD/serializationData.cpp \
	$$PWD/private/sceneGridHandler.cpp \
	$$PWD/private/resizeHandler.cpp \
	$$PWD/private/umlPortHandler.cpp \
	$$PWD/private/borderChecker.cpp \
	$$PWD/private/portHandler.cpp \
	$$PWD/private/copyHandler.cpp \
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
	$$PWD/commands/createAndUpdateGroupCommand.cpp \
	$$PWD/commands/multipleRemoveAndUpdateCommand.cpp \
	$$PWD/commands/resizeCommand.cpp \
	$$PWD/commands/foldCommand.cpp \
	$$PWD/commands/reshapeEdgeCommand.cpp \
	$$PWD/commands/expandCommand.cpp \
	$$PWD/miniMap/miniMap.cpp \
	$$PWD/miniMap/miniMapButton.cpp \
	$$PWD/miniMap/miniMapShell.cpp

RESOURCES += \
	$$PWD/contextIcons.qrc \
