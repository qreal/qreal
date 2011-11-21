#include "sceneGridHandler.h"
#include "nodeElement.h"
#include "../view/editorViewScene.h"

namespace {
// magic constants
const int widthLineX = 1500;
const int widthLineY = 1100;
}

SceneGridHandler::SceneGridHandler(NodeElement *node) : mNode(node)
{
	mGuidesPen = QPen(Qt::black, 0.25, Qt::DashLine);
	mShowAlignment = SettingsManager::value("ShowAlignment", true).toBool();
	mSwitchGrid = SettingsManager::value("ActivateGrid", true).toBool();
	mSwitchAlignment = SettingsManager::value("ActivateAlignment", true).toBool();
}

void SceneGridHandler::delUnusedLines()
{
	foreach (QGraphicsLineItem *lineItem, mLines) {
		mNode->scene()->removeItem(lineItem);
	}
	mLines.clear();
}

void SceneGridHandler::drawLineY(qreal pointY, qreal myX)
{
	QRectF const sceneRect = mNode->scene()->sceneRect();

	qreal const x1 = qMax(myX - widthLineY / 2, sceneRect.x() + 10);
	qreal const x2 = qMin(myX + widthLineY / 2, sceneRect.x() + sceneRect.width() - 10);

	QLineF const line(x1, pointY, x2, pointY);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QGraphicsLineItem *lineItem, mLines) {
		if (qAbs(lineItem->line().y1() - line.y1()) < indistinguishabilitySpace
				&& lineItem->line().y2() == lineItem->line().y1())
		{
			return;
		}
	}
	mLines.push_back(mNode->scene()->addLine(line, mGuidesPen));
}

void SceneGridHandler::drawLineX(qreal pointX, qreal myY)
{
	QRectF const sceneRect = mNode->scene()->sceneRect();

	qreal const y1 = qMax(myY - widthLineY / 2, sceneRect.y() + 10);
	qreal const y2 = qMin(myY + widthLineY / 2, sceneRect.y() + sceneRect.height() - 10);

	QLineF const line(pointX, y1, pointX, y2);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QGraphicsLineItem *lineItem, mLines) {
		if (qAbs(lineItem->line().x1() - line.x1()) < indistinguishabilitySpace
				&& lineItem->line().x2() == lineItem->line().x1())
		{
			return;
		}
	}

	mLines.push_back(mNode->scene()->addLine(line, mGuidesPen));
}

// checking whether we should align with the vertical line or not
bool SceneGridHandler::makeJumpX(qreal deltaX, qreal pointX)
{
	if (mSwitchAlignment && deltaX <= radiusJump) {
		mNode->setX(pointX - mNode->contentsRect().x());
		return true;
	}
	return false;
}

// checking whether we should align with the horizontal line or not
bool SceneGridHandler::makeJumpY(qreal deltaY, qreal pointY)
{
	if (mSwitchAlignment && deltaY <= radiusJump) {
		mNode->setY(pointY - mNode->contentsRect().y());
		return true;
	}
	return false;
}

// build a vertical line: draw it and check for alignment
void SceneGridHandler::buildLineX(qreal deltaX
		, qreal pointX, qreal correctionX, qreal &myX1, qreal &myX2, qreal myY)
{
	if (deltaX > radius) {
		return;
	}

	if (mShowAlignment) {
		drawLineX(pointX, myY);
	}
	if (makeJumpX(deltaX, pointX - correctionX)) {
		myX1 = recalculateX1();
		myX2 = recalculateX2(myX1);
	}
}

// build a horizontal line: draw it and check for alignment
void SceneGridHandler::buildLineY(qreal deltaY
		, qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, qreal myX)
{
	if (deltaY > radius) {
		return;
	}

	if (mShowAlignment) {
		drawLineY(pointY, myX);
	}
	if (makeJumpY(deltaY, pointY - correctionY)) {
		myY1 = recalculateY1();
		myY2 = recalculateY2(myY1);
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
	int oneKoef = koef != 0 ? koef / qAbs(koef) : 0;

	if (qAbs(qAbs(myX) - qAbs(koef) * indexGrid) <= indexGrid / 2) {
		mNode->setX(koef * indexGrid);
		mNode->adjustLinks();
	} else if (qAbs(qAbs(myX) - (qAbs(koef) + 1) * indexGrid) < indexGrid / 2) {
		mNode->setX((koef + oneKoef) * indexGrid);
		mNode->adjustLinks();
	}
}

// move element horizontally according to the grid
void SceneGridHandler::makeGridMovingY(qreal myY, int koef, int indexGrid)
{
	int oneKoef = koef != 0 ? koef / qAbs(koef) : 0;

	if (qAbs(qAbs(myY) - qAbs(koef) * indexGrid) <= indexGrid / 2) {
		mNode->setY(koef * indexGrid);
		mNode->adjustLinks();
	} else if (qAbs(qAbs(myY) - (qAbs(koef) + 1) * indexGrid) < indexGrid / 2) {
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

QList<QGraphicsItem *> SceneGridHandler::getAdjancedNodes()
{
	QPointF const nodeScenePos = mNode->scenePos();
	QRectF const contentsRect = mNode->contentsRect();

	// verical
	QList<QGraphicsItem *> listX = mNode->scene()->items(nodeScenePos.x(), 0
			, nodeScenePos.x() + contentsRect.width()
			, widthLineY
			, Qt::IntersectsItemBoundingRect, Qt::AscendingOrder);
	// horizontal
	QList<QGraphicsItem *> listY = mNode->scene()->items(0, nodeScenePos.y()
			, widthLineX
			, nodeScenePos.y() + contentsRect.height()
			, Qt::IntersectsItemBoundingRect, Qt::AscendingOrder);

	return listX + listY;
}

void SceneGridHandler::alignToGrid()
{
	if (!mSwitchGrid) {
		return;
	}
	int const indexGrid = SettingsManager::value("IndexGrid", 50).toInt();

	QPointF const nodeScenePos = mNode->scenePos();
	QRectF const contentsRect = mNode->contentsRect();

	qreal myX1 = nodeScenePos.x() + contentsRect.x();
	qreal myY1 = nodeScenePos.y() + contentsRect.y();

	int coefX = static_cast<int>(myX1) / indexGrid;
	int coefY = static_cast<int>(myY1) / indexGrid;

	makeGridMovingX(myX1, coefX, indexGrid);
	makeGridMovingY(myY1, coefY, indexGrid);

	myX1 = nodeScenePos.x() + contentsRect.x();
	myY1 = nodeScenePos.y() + contentsRect.y();
}

void SceneGridHandler::drawGuides()
{
	QPointF const nodeScenePos = mNode->scenePos();
	QRectF const contentsRect = mNode->contentsRect();

	delUnusedLines();
	qreal myX1 = nodeScenePos.x() + contentsRect.x();
	qreal myY1 = nodeScenePos.y() + contentsRect.y();
	qreal myX2 = myX1 + contentsRect.width();
	qreal myY2 = myY1 + contentsRect.height();

	QList<QGraphicsItem *> list = getAdjancedNodes();

	foreach (QGraphicsItem *graphicsItem, list) {
		NodeElement *item = dynamic_cast<NodeElement*>(graphicsItem);
		if (item == NULL || item->parentItem() != NULL || item == mNode) {
			continue;
		}

		QPointF const point = item->scenePos();
		QRectF const contents = item->contentsRect();

		qreal const pointX1 = point.x() + contents.x() - spacing;
		qreal const pointY1  = point.y() + contents.y() - spacing;
		qreal const pointX2 = pointX1  + contents.width() + 2 * spacing;
		qreal const pointY2  = pointY1 + contents.height() + 2 * spacing;

		if (pointX1 != myX1 || pointY1 != myY1) {
			qreal const deltaY1 = qAbs(pointY1 - myY1);
			qreal const deltaY2 = qAbs(pointY2 - myY2);
			qreal const deltaX1 = qAbs(pointX1 - myX1);
			qreal const deltaX2 = qAbs(pointX2 - myX2);

			buildLineY(deltaY1, pointY1, 0, myY1, myY2, myX1);
			buildLineY(deltaY2, pointY2, contentsRect.height(), myY1, myY2, myX1);

			buildLineX(deltaX1, pointX1, 0, myX1, myX2, myY1);
			buildLineX(deltaX2, pointX2, contentsRect.width(), myX1, myX2, myY1);

			buildLineY(qAbs(pointY1 - myY2), pointY1, contentsRect.height(), myY1, myY2, myX1);
			buildLineX(qAbs(pointX1 - myX2), pointX1, contentsRect.width(), myX1, myX2, myY1);

			buildLineY(qAbs(pointY2 - myY1), pointY2, 0, myY1, myY2, myX1);
			buildLineX(qAbs(pointX2 - myX1), pointX2, 0, myX1, myX2, myY1);
		}
	}
}

void SceneGridHandler::mouseMoveEvent()
{
	NodeElement *parItem = dynamic_cast<NodeElement*>(mNode->parentItem());
	if (parItem != NULL) {
		return;
	}

	alignToGrid();
	drawGuides();
	mNode->adjustLinks();
}
