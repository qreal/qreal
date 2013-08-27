#include "copyHandler.h"

#include "umllib/nodeElement.h"
#include "view/editorViewScene.h"

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

void CopyHandler::copyChildren(NodeElement const &destination, NodeElement const &source) const
{
	foreach (QGraphicsItem * const child, source.childItems()) {
		NodeElement * const element = dynamic_cast<NodeElement *>(child);
		if (!element) {
			continue;
		}

		CopyHandler copyHandler(*element, mGraphicalAssistApi);
		NodeElement const * const copyOfChild = copyHandler.clone();
		mGraphicalAssistApi.changeParent(copyOfChild->id(), destination.id(), element->pos());
	}
}

void CopyHandler::copyProperties(NodeElement const &destination, NodeElement const &source) const
{
	mGraphicalAssistApi.copyProperties(destination.id(), source.id());
}

void CopyHandler::copyEdges(NodeElement const &destination, NodeElement const &source) const
{
	Q_UNUSED(source);
	Q_UNUSED(destination);
}
