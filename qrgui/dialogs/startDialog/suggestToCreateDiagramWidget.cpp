#include "suggestToCreateDiagramWidget.h"
#include "../../mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent)
		: ListWidget(parent)
		, mMainWindow(mainWindow)
{
	foreach(Id const &editor, mMainWindow->manager()->editors()) {
		Id editorTmpId = Id::loadFromString("qrm:/" + editor.editor());
		foreach(Id const &diagram, mMainWindow->manager()->diagrams(editorTmpId)) {
			addItem(editor, diagram);
		}
	}
	highlightFirstItem();
}

void SuggestToCreateDiagramWidget::addItem(Id const &editor, Id const &diagram)
{
	EditorInterface *editorInterface = mMainWindow->manager()->editorInterface(editor.editor());

	QString const diagramName = editorInterface->diagramName(diagram.diagram());
	QString const diagramNodeName = editorInterface->diagramNodeName(diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return;
	}
	ListWidget::addItem(diagramName
			, "qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName
			, tr("editor: ") + editor.editor() + tr(", diagram: ") + diagram.diagram());
}
