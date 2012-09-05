#include "umlPortHandler.h"
#include "../nodeElement.h"
#include "borderChecker.h"

UmlPortHandler::UmlPortHandler(NodeElement * const node)
	: mNode(node), mBelongsToHorizontalBorders(true)
{
}

void UmlPortHandler::handleMoveEvent(bool const leftPressed
		, QPointF &pos, QPointF const &scenePos
		, NodeElement *&parentNode)
{
	if (!mNode || !leftPressed) {
		return;
	}

	if (pos == QPointF(0,0)) {
		pos = mNode->pos();
	}

	QGraphicsItem* const item = mNode->scene()->items(scenePos).value(1);
	NodeElement* const actionItem = dynamic_cast<NodeElement * const>(item);
	BorderChecker const actionItemBorderChecker(actionItem);
	QPointF posInItem = QPointF(0, 0);
	if (actionItem && ((actionItem == parentNode) || (!parentNode))) {
		if (actionItem->canHavePorts()) {
			posInItem = actionItem->mapFromScene(scenePos);

			bool const checkLowerBorder = actionItemBorderChecker.checkLowerBorder(posInItem);
			bool const checkUpperBorder = actionItemBorderChecker.checkUpperBorder(posInItem);
			bool const checkRightBorder = actionItemBorderChecker.checkRightBorder(posInItem);
			bool const checkLeftBorder = actionItemBorderChecker.checkLeftBorder(posInItem);
			bool const checkBorders = checkLowerBorder || checkUpperBorder
				|| checkRightBorder || checkLeftBorder;

			if (checkBorders) {
				mNode->setParentItem(actionItem);
				parentNode = actionItem;
				pos = mNode->pos();
				mBelongsToHorizontalBorders = checkLowerBorder || checkUpperBorder;
			} else {
				handleHorizontalBorders(actionItem, parentNode, pos, posInItem);
			}
		}
	} else if ((parentNode) && (parentNode->canHavePorts())) {
			mNode->setPos(pos);
			if (parentNode) {
				posInItem = parentNode->mapFromScene(scenePos);
				handleHorizontalBorders(parentNode, parentNode, pos, posInItem);
			}
	}
}

void UmlPortHandler::handleHorizontalBorders(
		const NodeElement * const tmpNode, const NodeElement * const parentNode
		, QPointF const &pos, QPointF const &posInItem) const
{
	QPointF newPos = pos;
	BorderChecker const parentNodeBorderChecker(parentNode);
	if (mBelongsToHorizontalBorders) {
		// tmpNode->borderValues()[0] == xHor of tmpNode. (mXHor field of BorderChecker(tmpNode))
		if (parentNodeBorderChecker.checkNoBorderY(posInItem, tmpNode->borderValues()[0])) {
			newPos.setX(posInItem.x());
		}
	} else {
		// tmpNode->borderValues()[3] == yVert of tmpNode. (mYVert field of BorderChecker(tmpNode))
		if (parentNodeBorderChecker.checkNoBorderX(posInItem, tmpNode->borderValues()[3])) {
			newPos.setY(posInItem.y());
		}
	}
	mNode->setPos(newPos);
}
