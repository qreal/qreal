#include "umlPortHandler.h"
#include "nodeElement.h"

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
	UmlPortHandler const actionItemPortHandler(actionItem);
	QPointF posInItem = QPointF(0, 0);
	if (actionItem && ((actionItem == parentNode) || (!parentNode))) {
		if (actionItem->canHavePorts()) {
			posInItem = actionItem->mapFromScene(scenePos);
			QList<double> const list = actionItem->borderValues();
			double const xHor = list[0];
			double const yHor = list[1];
			double const xVert = list[2];
			double const yVert = list[3];

			bool const checkLowerBorder = actionItemPortHandler.checkLowerBorder(posInItem, xHor, yHor);
			bool const checkUpperBorder = actionItemPortHandler.checkUpperBorder(posInItem, xHor, yHor);
			bool const checkRightBorder = actionItemPortHandler.checkRightBorder(posInItem, xVert, yVert);
			bool const checkLeftBorder = actionItemPortHandler.checkLeftBorder(posInItem, xVert, yVert);
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
	if (mBelongsToHorizontalBorders) {
		double const xHor = list[0];

		if (parentNode->checkNoBorderY(posInItem, xHor)) {
			newPos.setX(posInItem.x());
		}
	} else {
		double const yVert = list[3];

		if (parentNode->checkNoBorderX(posInItem, yVert)) {
			newPos.setY(posInItem.y());
		}
	}
	mNode->setPos(newPos);
}

bool UmlPortHandler::checkLowerBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x) && (currentY >= rc.y() + rc.height() - y)
			&& (currentY <= rc.y() + rc.height() + y);
}

bool UmlPortHandler::checkUpperBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x) && (currentY >= rc.y() - y)
			&& (currentY <= rc.y() + y);
}

bool UmlPortHandler::checkLeftBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() - x) && (currentX <= rc.x() + x) && (currentY >= rc.y() + y)
			&& (currentY <= rc.y() + rc.height() - y);
}

bool UmlPortHandler::checkRightBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + rc.width() - x) && (currentX <= rc.x() + rc.width() + x) && (currentY >= rc.y() + y)
			&& (currentY <= rc.y() + rc.height() - y);
}

bool UmlPortHandler::checkNoBorderX(QPointF const &point, double const y) const
{
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentY >= rc.y() + y) && (currentY <= rc.y() + rc.height() - y);
}

bool UmlPortHandler::checkNoBorderY(QPointF const &point, double const x) const
{
	double currentX = point.x();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x);
}
