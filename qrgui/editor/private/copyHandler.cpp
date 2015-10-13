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

#include "copyHandler.h"

#include "editor/nodeElement.h"
#include "editor/editorViewScene.h"

using namespace qReal;
using namespace models;

CopyHandler::CopyHandler(NodeElement &copyingElement, GraphicalModelAssistApi &graphicalAssistApi)
	: mNode(copyingElement), mGraphicalAssistApi(graphicalAssistApi)
{
}

NodeElement *CopyHandler::clone(bool toCursorPos, bool searchForParents)
{
	EditorViewScene *evscene = dynamic_cast<EditorViewScene *>(mNode.scene());

	qReal::Id typeId = mNode.id().type();
	qReal::Id resultId = evscene->createElement(typeId.toString(), QPointF(), searchForParents);

	NodeElement *result = dynamic_cast<NodeElement *>(evscene->getElem(resultId));

	copyProperties(*result, mNode);
	copyChildren(*result, mNode);
	QRectF contents = mNode.contentsRect();
	if (toCursorPos) {
		contents.moveTo(evscene->getMousePos());
		result->setGeometry(contents);
		result->storeGeometry();
	}
	else {
		contents.moveTo(mNode.pos());
		result->setGeometry(contents);
	}

	return result;
}

void CopyHandler::copyChildren(const NodeElement &destination, const NodeElement &source) const
{
	foreach (QGraphicsItem * const child, source.childItems()) {
		NodeElement * const element = dynamic_cast<NodeElement *>(child);
		if (!element) {
			continue;
		}

		CopyHandler copyHandler(*element, mGraphicalAssistApi);
		const NodeElement * const copyOfChild = copyHandler.clone();
		mGraphicalAssistApi.changeParent(copyOfChild->id(), destination.id(), element->pos());
	}
}

void CopyHandler::copyProperties(const NodeElement &destination, const NodeElement &source) const
{
	mGraphicalAssistApi.copyProperties(destination.id(), source.id());
}

void CopyHandler::copyEdges(const NodeElement &destination, const NodeElement &source) const
{
	Q_UNUSED(source);
	Q_UNUSED(destination);
}
