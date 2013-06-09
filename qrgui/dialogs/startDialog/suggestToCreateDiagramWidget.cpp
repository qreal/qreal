#include "suggestToCreateDiagramWidget.h"
#include "../../mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent)
		: ListWidget(parent)
		, mMainWindow(mainWindow)
{
	foreach (Id const &editor, mMainWindow->editorManager().editors()) {
		Id editorTmpId = Id::loadFromString("qrm:/" + editor.editor());
		foreach(Id const &diagram, mMainWindow->editorManager().diagrams(editorTmpId)) {
			addItem(editor, diagram);
		}
	}
	highlightFirstItem();
}

void SuggestToCreateDiagramWidget::addItem(Id const &editor, Id const &diagram)
{
	EditorManagerInterface const &editorManagerInterface = mMainWindow->editorManager();

	QString const diagramName = editorManagerInterface.diagramName(editor.editor(), diagram.diagram());
	QString const diagramNodeName = editorManagerInterface.diagramNodeName(editor.editor(), diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return;
	}

	ListWidget::addItem(diagramName
			, "qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName
			, tr("editor: ") + editor.editor() + tr(", diagram: ") + diagram.diagram());
}
