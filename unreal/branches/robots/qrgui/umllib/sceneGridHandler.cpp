#include "sceneGridHandler.h"
#include "uml_nodeelement.h"
#include "../view/editorviewscene.h"

#include <QtCore/QSettings>

using namespace UML;

SceneGridHandler::SceneGridHandler(NodeElement *node)
{
	QSettings settings("SPbSU", "QReal");
	mNode = node;
	mShowAlignment = settings.value("ShowAlignment", false).toBool();
	mSwitchGrid = settings.value("ActivateGrid", false).toBool();
	mSwitchAlignment = settings.value("ActivateAlignment", false).toBool();
}

void SceneGridHandler::delUnusedLines()
{
	for (int i = mLines.size() - 1; i >= 0; i--) {
		mLines[i]->hide();
		mNode->scene()->removeItem(mLines[i]);
		mLines.pop_back();
	}
}

//drawing a horizontal line
void SceneGridHandler::drawLineY(qreal pointY, qreal myX)
{
	bool lineIsFound = false;
	qreal x1 = myX - widthLineX / 2;
	qreal x2 = myX + widthLineX / 2;
	if (myX - mNode->scene()->sceneRect().x() < widthLineX / 2)
		x1 = mNode->scene()->sceneRect().x() + 10;
	if (mNode->scene()->sceneRect().x() + mNode->scene()->sceneRect().width() - myX < widthLineX / 2)
		x2 = mNode->scene()->sceneRect().x() + mNode->scene()->sceneRect().width() - 10;
	QLineF line(x1, pointY, x2, pointY);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QGraphicsLineItem* lineItem, mLines) {
		if (lineItem->line().y1() == line.y1() && lineItem->line().y2() == line.y2())
			lineIsFound = true;
	}
	if (!lineIsFound)
		mLines.push_back(mNode->scene()->addLine(line, QPen(Qt::black, 0.25, Qt::DashLine)));
}

//drawing a vertical line
void SceneGridHandler::drawLineX(qreal pointX, qreal myY)
{
	bool lineIsFound = false;
	qreal y1 = myY - widthLineY / 2;
	qreal y2 = myY + widthLineY / 2;
	if (myY - mNode->scene()->sceneRect().y() < widthLineY / 2)
		y1 = mNode->scene()->sceneRect().y() + 10;
	if (mNode->scene()->sceneRect().y() + mNode->scene()->sceneRect().height() - myY < widthLineY / 2)
		y2 = mNode->scene()->sceneRect().y() + mNode->scene()->sceneRect().height() - 10;
	QLineF line(pointX, y1, pointX, y2);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QGraphicsLineItem* lineItem, mLines) {
		if (lineItem->line().x1() == line.x1() && lineItem->line().x2() == line.x2())
			lineIsFound = true;
	}
	if (!lineIsFound)
		mLines.push_back(mNode->scene()->addLine(line, QPen(Qt::black, 0.25, Qt::DashLine)));
}

// checking whether we should align with the vertical line or not
bool SceneGridHandler::makeJumpX(qreal deltaX, qreal radiusJump, qreal pointX)
{
	if (mSwitchAlignment && deltaX <= radiusJump) {
		mNode->setX(pointX - mNode->boundingRect().x());
		mNode->adjustLinks();
		return true;
	}
	return false;
}

// checking whether we should align with the horizontal line or not
bool SceneGridHandler::makeJumpY(qreal deltaY, qreal radiusJump, qreal pointY)
{
	if (mSwitchAlignment && deltaY <= radiusJump) {
		mNode->setY(pointY - mNode->boundingRect().y());
		mNode->adjustLinks();
		return true;
	}
	return false;
}

// build a vertical line: draw it and check for alignment
void SceneGridHandler::buildLineX(qreal deltaX, qreal radius, bool doAlways,
	qreal radiusJump, qreal pointX, qreal correctionX, qreal &myX1, qreal &myX2, qreal myY)
{
	if (deltaX <= radius || doAlways) {
		if (mShowAlignment)
			drawLineX(pointX, myY);
		if (makeJumpX(deltaX, radiusJump, pointX - correctionX)) {
			myX1 = recalculateX1();
			myX2 = recalculateX2(myX1);
		}
	}
}

// build a horizontal line: draw it and check for alignment
void SceneGridHandler::buildLineY(qreal deltaY, qreal radius, bool doAlways,
	qreal radiusJump, qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, qreal myX)
{
	if (deltaY <= radius || doAlways) {
		if (mShowAlignment)
			drawLineY(pointY, myX);
		if (makeJumpY(deltaY, radiusJump, pointY - correctionY)) {
			myY1 = recalculateY1();
			myY2 = recalculateY2(myY1);
		}
	}
}

qreal SceneGridHandler::recalculateX1()
{
	return mNode->scenePos().x() + mNode->boundingRect().x();
}

qreal SceneGridHandler::recalculateX2(qreal myX1)
{
	return myX1 + mNode->boundingRect().width();
}

qreal SceneGridHandler::recalculateY1()
{
	return mNode->scenePos().y() + mNode->boundingRect().y();
}

qreal SceneGridHandler::recalculateY2(qreal myY1)
{
	return myY1 + mNode->boundingRect().height();
}

// move element vertically according to the grid
void SceneGridHandler::makeGridMovingX(qreal myX, int koef, int indexGrid)
{
	int oneKoef = 0;
	if (koef != 0)
		oneKoef = koef / qAbs(koef);
	if (qAbs(qAbs(myX) - qAbs(koef) * indexGrid) <= indexGrid / 2) {
		mNode->setX(koef * indexGrid);
		mNode->adjustLinks();
	}
	else if (qAbs(qAbs(myX) - (qAbs(koef) + 1) * indexGrid) < indexGrid / 2) {
		mNode->setX((koef + oneKoef) * indexGrid);
		mNode->adjustLinks();
	}
}

// move element horizontally according to the grid
void SceneGridHandler::makeGridMovingY(qreal myY, int koef, int indexGrid)
{
	int oneKoef = 0;
	if (koef != 0)
		oneKoef = koef / qAbs(koef);
	if (qAbs(qAbs(myY) - qAbs(koef) * indexGrid) <= indexGrid / 2) {
		mNode->setY(koef * indexGrid);
		mNode->adjustLinks();
	}
	else if (qAbs(qAbs(myY) - (qAbs(koef) + 1) * indexGrid) < indexGrid / 2) {
		mNode->setY((koef + oneKoef) * indexGrid);
		mNode->adjustLinks();
	}
}

void SceneGridHandler::setShowAlignmentMode(bool mode)
{
	mShowAlignment = mode;
}

void SceneGridHandler::setGridMode(bool mode)
{
	mSwitchGrid = mode;
}

void SceneGridHandler::setAlignmentMode(bool mode)
{
	mSwitchAlignment = mode;
}

void SceneGridHandler::mouseMoveEvent()
{
	NodeElement* parItem = dynamic_cast<NodeElement*>(mNode->parentItem());
	if(parItem == NULL) {
		qreal myX1 = mNode->scenePos().x() + mNode->boundingRect().x();
		qreal myY1 = mNode->scenePos().y() + mNode->boundingRect().y();

		if (mSwitchGrid) {
			int coefX = static_cast<int>(myX1) / indexGrid;
			int coefY = static_cast<int>(myY1) / indexGrid;

			makeGridMovingX(myX1, coefX, indexGrid);
			makeGridMovingY(myY1, coefY, indexGrid);

			myX1 = mNode->scenePos().x() + mNode->boundingRect().x();
			myY1 = mNode->scenePos().y() + mNode->boundingRect().y();
		}

		qreal myX2 = myX1 + mNode->boundingRect().width();
		qreal myY2 = myY1 + mNode->boundingRect().height();

		qreal radius = 20;
		qreal radiusJump = 10;

		QList<QGraphicsItem *> list = mNode->scene()->items(mNode->scenePos().x() - widthLineX / 2
								, mNode->scenePos().y() - widthLineY / 2, widthLineX, widthLineY
								, Qt::IntersectsItemBoundingRect, Qt::AscendingOrder);
		delUnusedLines();
		foreach (QGraphicsItem *graphicsItem, list) {
			NodeElement* item = dynamic_cast<NodeElement*>(graphicsItem);
			if (item == NULL || item->parentItem() != NULL)
				continue;
			QPointF point = item->scenePos();
			qreal pointX1 = point.x() + item->boundingRect().x();
			qreal pointY1  = point.y() + item->boundingRect().y();
			qreal pointX2 = pointX1  + item->boundingRect().width();
			qreal pointY2  = pointY1 + item->boundingRect().height();

			if (pointX1 != myX1 || pointY1 != myY1) {
				qreal deltaY1 = qAbs(pointY1 - myY1);
				qreal deltaY2 = qAbs(pointY2 - myY2);
				qreal deltaX1 = qAbs(pointX1 - myX1);
				qreal deltaX2 = qAbs(pointX2 - myX2);
				if (deltaY1 <= radius || deltaY2 <= radius) {
					buildLineY(deltaY1, radius, true, radiusJump, pointY1, 0, myY1, myY2, myX1);
					buildLineY(deltaY2, radius, true, radiusJump, pointY2,
						mNode->boundingRect().height(), myY1, myY2, myX1);
				}
				if (deltaX1 <= radius || deltaX2 <= radius) {
					buildLineX(deltaX1, radius, true, radiusJump, pointX1, 0, myX1, myX2, myY1);
					buildLineX(deltaX2, radius, true, radiusJump, pointX2,
						mNode->boundingRect().width(), myX1, myX2, myY1);
				}
				buildLineY(qAbs(pointY1 - myY2), radius, false, radiusJump, pointY1,
					mNode->boundingRect().height(), myY1, myY2, myX1);
				buildLineX(qAbs(pointX1 - myX2), radius, false, radiusJump, pointX1,
					mNode->boundingRect().width(), myX1, myX2, myY1);
				buildLineY(qAbs(pointY2 - myY1), radius, false, radiusJump, pointY2,
					0, myY1, myY2, myX1);
				buildLineX(qAbs(pointX2 - myX1), radius, false, radiusJump, pointX2,
					0, myX1, myX2, myY1);
			}
		}
	}
}
