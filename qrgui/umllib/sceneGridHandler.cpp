#include "sceneGridHandler.h"
#include "nodeElement.h"
#include "../view/editorViewScene.h"

using namespace qReal;

namespace {
// TODO: find a way to remove it
// magic constants
const int widthLineX = 15000;
const int widthLineY = 11000;
}

SceneGridHandler::SceneGridHandler(NodeElement *node)
		: mNode(node), mGuidesPixmap(NULL)
{
	mGuidesPen = QPen(QColor(0, 0, 0, 42), 1, Qt::DashLine);
	mShowAlignment = SettingsManager::value("ShowAlignment").toBool();
	mSwitchGrid = SettingsManager::value("ActivateGrid").toBool();
	mSwitchAlignment = SettingsManager::value("ActivateAlignment").toBool();
}

SceneGridHandler::~SceneGridHandler()
{
	delete mGuidesPixmap;
}

void SceneGridHandler::delUnusedLines()
{
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(mNode->scene());
	evScene->deleteFromForeground(mGuidesPixmap);
	delete mGuidesPixmap;
	mGuidesPixmap = NULL;
	mLines.clear();
}

void SceneGridHandler::drawLineY(qreal pointY, QRectF const &sceneRect)
{
	pointY -= sceneRect.y();
	QLineF const newLine(0, pointY, sceneRect.width(), pointY);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QLineF const &line, mLines) {
		if (qAbs(line.y1() - newLine.y1()) < indistinguishabilitySpace
				&& line.y2() == line.y1())
		{
			return;
		}
	}

	mLines.push_back(newLine);
}

void SceneGridHandler::drawLineX(qreal pointX, QRectF const &sceneRect)
{
	pointX -= sceneRect.x();
	QLineF const newLine(pointX, 0, pointX, sceneRect.height());

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QLineF const &line, mLines) {
		if (qAbs(line.x1() - newLine.x1()) < indistinguishabilitySpace
				&& line.x2() == line.x1())
		{
			return;
		}
	}

	mLines.push_back(newLine);
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
		, qreal pointX, qreal correctionX, qreal &myX1, qreal &myX2, QRectF const &sceneRect)
{
	if (deltaX > radius) {
		return;
	}

	if (mShowAlignment) {
		drawLineX(pointX, sceneRect);
	}
	if (makeJumpX(deltaX, pointX - correctionX)) {
		myX1 = recalculateX1();
		myX2 = recalculateX2(myX1);
	}
}

// build a horizontal line: draw it and check for alignment
void SceneGridHandler::buildLineY(qreal deltaY
		, qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, QRectF const &sceneRect)
{
	if (deltaY > radius) {
		return;
	}

	if (mShowAlignment) {
		drawLineY(pointY, sceneRect);
	}
	if (makeJumpY(deltaY, pointY - correctionY)) {
		myY1 = recalculateY1();
		myY2 = recalculateY2(myY1);
	}
}

qreal SceneGridHandler::recalculateX1() const
{
	return mNode->scenePos().x() + mNode->boundingRect().x();
}

qreal SceneGridHandler::recalculateX2(qreal myX1) const
{
	return myX1 + mNode->boundingRect().width();
}

qreal SceneGridHandler::recalculateY1() const
{
	return mNode->scenePos().y() + mNode->boundingRect().y();
}

qreal SceneGridHandler::recalculateY2(qreal myY1) const
{
	return myY1 + mNode->boundingRect().height();
}

qreal SceneGridHandler::alignedCoordinate(qreal coord, int coef, int indexGrid)
{
	int const coefSign = coef != 0 ? coef / qAbs(coef) : 0;

	if (qAbs(qAbs(coord) - qAbs(coef) * indexGrid) <= indexGrid / 2) {
		return coef * indexGrid;
	} else if (qAbs(qAbs(coord) - (qAbs(coef) + 1) * indexGrid) < indexGrid / 2) {
		return (coef + coefSign) * indexGrid;
	}
	return coord;
}

void SceneGridHandler::makeGridMovingX(qreal myX, int coef, int indexGrid)
{
	mNode->setX(alignedCoordinate(myX, coef, indexGrid));
	mNode->adjustLinks();
}

void SceneGridHandler::makeGridMovingY(qreal myY, int coef, int indexGrid)
{
	mNode->setY(alignedCoordinate(myY, coef, indexGrid));
	mNode->adjustLinks();
}

qreal SceneGridHandler::makeGridAlignment(qreal coord)
{
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();
	int const coef = static_cast<int>(coord) / indexGrid;
	return alignedCoordinate(coord, coef, indexGrid);
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

QList<QGraphicsItem *> SceneGridHandler::getAdjancedNodes() const
{
	QPointF const nodeScenePos = mNode->scenePos();
	QRectF const contentsRect = mNode->contentsRect();

	// verical
	QList<QGraphicsItem *> listX = mNode->scene()->items(nodeScenePos.x(), 0
			, contentsRect.width(), widthLineY
			, Qt::IntersectsItemBoundingRect);

	// horizontal
	QList<QGraphicsItem *> listY = mNode->scene()->items(0, nodeScenePos.y()
			, widthLineX, contentsRect.height()
			, Qt::IntersectsItemBoundingRect);

	return listX + listY;
}

void SceneGridHandler::alignToGrid()
{
	if (!mSwitchGrid || mNode->parentItem()) {
		return;
	}
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();

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
	QRectF const sceneRect = mNode->scene()->sceneRect();

	delUnusedLines();

	QList<QGraphicsItem *> list = getAdjancedNodes();

	qreal myX1 = nodeScenePos.x() + contentsRect.x();
	qreal myY1 = nodeScenePos.y() + contentsRect.y();
	qreal myX2 = myX1 + contentsRect.width();
	qreal myY2 = myY1 + contentsRect.height();

	foreach (QGraphicsItem *graphicsItem, list) {
		NodeElement *item = dynamic_cast<NodeElement *>(graphicsItem);
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

			buildLineY(deltaY1, pointY1, 0, myY1, myY2, sceneRect);
			buildLineY(deltaY2, pointY2, contentsRect.height(), myY1, myY2, sceneRect);

			buildLineX(deltaX1, pointX1, 0, myX1, myX2, sceneRect);
			buildLineX(deltaX2, pointX2, contentsRect.width(), myX1, myX2, sceneRect);

			buildLineY(qAbs(pointY1 - myY2), pointY1, contentsRect.height(), myY1, myY2, sceneRect);
			buildLineX(qAbs(pointX1 - myX2), pointX1, contentsRect.width(), myX1, myX2, sceneRect);

			buildLineY(qAbs(pointY2 - myY1), pointY2, 0, myY1, myY2, sceneRect);
			buildLineX(qAbs(pointX2 - myX1), pointX2, 0, myX1, myX2, sceneRect);
		}
	}


	if (mLines.size()) {
		EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(mNode->scene());

		mGuidesPixmap = new QPixmap(sceneRect.width(), sceneRect.height());
		mGuidesPixmap->fill(Qt::transparent);

		QPainter painter(mGuidesPixmap);
		painter.setPen(mGuidesPen);
		painter.drawLines(mLines);
		evScene->putOnForeground(mGuidesPixmap);
	}
}

void SceneGridHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
	NodeElement *parItem = dynamic_cast<NodeElement*>(mNode->parentItem());
	if (parItem != NULL) {
		return;
	}

	foreach (QGraphicsItem *item, mNode->scene()->items()) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (e && e->isSelected()) {
			e->alignToGrid();
		}
	}
	drawGuides();
	mNode->adjustLinks();
}
