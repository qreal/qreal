/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
