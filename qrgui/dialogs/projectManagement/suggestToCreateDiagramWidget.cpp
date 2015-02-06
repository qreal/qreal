#include "suggestToCreateDiagramWidget.h"

#include <plugins/pluginManager/editorManagerInterface.h>

using namespace qReal;

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(const EditorManagerInterface &editorManager
		, QWidget *parent)
	: ListWidget(parent)
	, mEditorManager(editorManager)
{
	for (const Id &editor : mEditorManager.editors()) {
		const Id editorTmpId = Id::loadFromString("qrm:/" + editor.editor());
		for (const Id &diagram : mEditorManager.diagrams(editorTmpId)) {
			addItem(editor, diagram);
		}
	}

	highlightFirstItem();
}

void SuggestToCreateDiagramWidget::addItem(const Id &editor, const Id &diagram)
{
	const QString diagramName = mEditorManager.diagramName(editor.editor(), diagram.diagram());
	const QString diagramNodeName = mEditorManager.diagramNodeName(editor.editor(), diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return;
	}

	ListWidget::addItem(diagramName
			, "qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName
			, tr("editor: ") + editor.editor() + tr(", diagram: ") + diagram.diagram());
}
