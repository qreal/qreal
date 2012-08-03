#include "umlPortHandler.h"
#include "nodeElement.h"
#include "borderChecker.h"

UmlPortHandler::UmlPortHandler(NodeElement* const node)
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
	NodeElement* const actionItem = dynamic_cast<NodeElement* const>(item);
	BorderChecker const actionItemBorderChecker(actionItem);
	QPointF posInItem = QPointF(0, 0);
	if (actionItem && ((actionItem == parentNode) || (!parentNode))) {
		if (actionItem->canHavePorts()) {
			posInItem = actionItem->mapFromScene(scenePos);
			QList<double> const list = actionItem->borderValues();
			double const xHor = list[0];
			double const yHor = list[1];
			double const xVert = list[2];
			double const yVert = list[3];

			bool const checkLowerBorder = actionItemBorderChecker.checkLowerBorder(posInItem, xHor, yHor);
			bool const checkUpperBorder = actionItemBorderChecker.checkUpperBorder(posInItem, xHor, yHor);
			bool const checkRightBorder = actionItemBorderChecker.checkRightBorder(posInItem, xVert, yVert);
			bool const checkLeftBorder = actionItemBorderChecker.checkLeftBorder(posInItem, xVert, yVert);
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
		const NodeElement* const tmpNode, const NodeElement* const parentNode
		, QPointF const &pos, QPointF const &posInItem) const
{
	QList<double> const list = tmpNode->borderValues();
	
	QPointF newPos = pos;
	BorderChecker const parentNodeBorderChecker(parentNode);
	if (mBelongsToHorizontalBorders) {
		double const xHor = list[0];

		if (parentNodeBorderChecker.checkNoBorderY(posInItem, xHor)) {
			newPos.setX(posInItem.x());
		}
	} else {
		double const yVert = list[3];

		if (parentNodeBorderChecker.checkNoBorderX(posInItem, yVert)) {
			newPos.setY(posInItem.y());
		}
	}
	mNode->setPos(newPos);
}
