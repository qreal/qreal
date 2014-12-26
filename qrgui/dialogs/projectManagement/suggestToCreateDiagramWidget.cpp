#include "suggestToCreateDiagramWidget.h"

#include <plugins/pluginManager/editorManagerInterface.h>

using namespace qReal;

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(EditorManagerInterface const &editorManager
		, QWidget *parent)
	: ListWidget(parent)
	, mEditorManager(editorManager)
{
	for (Id const &editor : mEditorManager.editors()) {
		Id const editorTmpId = Id::loadFromString("qrm:/" + editor.editor());
		for (Id const &diagram : mEditorManager.diagrams(editorTmpId)) {
			addItem(editor, diagram);
		}
	}

	highlightFirstItem();
}

void SuggestToCreateDiagramWidget::addItem(Id const &editor, Id const &diagram)
{
	QString const diagramName = mEditorManager.diagramName(editor.editor(), diagram.diagram());
	QString const diagramNodeName = mEditorManager.diagramNodeName(editor.editor(), diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return;
	}

	ListWidget::addItem(diagramName
			, "qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName
			, tr("editor: ") + editor.editor() + tr(", diagram: ") + diagram.diagram());
}
