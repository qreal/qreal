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

#include "sceneGridHandler.h"

#include "editor/nodeElement.h"
#include "editor/editorViewScene.h"

using namespace qReal;
using namespace qReal::gui::editor;

namespace {
// TODO: find a way to remove it
// magic constants
const int widthLineX = 15000;
const int widthLineY = 11000;
}

SceneGridHandler::SceneGridHandler(NodeElement *node)
	: mNode(node)
{
	mGuidesPen = QPen(QColor(0, 0, 0, 42), 1, Qt::DashLine);
	mShowAlignment = SettingsManager::value("ShowAlignment").toBool();
	mSwitchGrid = SettingsManager::value("ActivateGrid").toBool();
	mSwitchAlignment = SettingsManager::value("ActivateAlignment").toBool();
}

SceneGridHandler::~SceneGridHandler()
{
}

void SceneGridHandler::deleteGuides()
{
	qDeleteAll(mGuides);
	mGuides.clear();
}

void SceneGridHandler::drawLineY(qreal pointY, const QRectF &sceneRect)
{
	QGraphicsLineItem * const newLine = new QGraphicsLineItem(sceneRect.x()
			, pointY, sceneRect.x() + sceneRect.width(), pointY);
	newLine->setPen(mGuidesPen);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	for (const QGraphicsLineItem *line : mGuides) {
		if (qAbs(line->line().y1() - newLine->line().y1()) < indistinguishabilitySpace
				&& line->line().y2() == line->line().y1())
		{
			delete newLine;
			return;
		}
	}

	mGuides << newLine;
	mNode->scene()->addItem(newLine);
}

void SceneGridHandler::drawLineX(qreal pointX, const QRectF &sceneRect)
{
	QGraphicsLineItem * const newLine = new QGraphicsLineItem(pointX
			, sceneRect.y(), pointX, sceneRect.y() + sceneRect.height());
	newLine->setPen(mGuidesPen);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	for (const QGraphicsLineItem *line : mGuides) {
		if (qAbs(line->line().x1() - newLine->line().x1()) < indistinguishabilitySpace
				&& line->line().x2() == line->line().x1())
		{
			delete newLine;
			return;
		}
	}

	mGuides << newLine;
	mNode->scene()->addItem(newLine);
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
		, qreal pointX, qreal correctionX, qreal &myX1, qreal &myX2, const QRectF &sceneRect)
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
		, qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, const QRectF &sceneRect)
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
	const int coefSign = coef != 0 ? coef / qAbs(coef) : 0;

	if (qAbs(qAbs(coord) - qAbs(coef) * indexGrid) <= indexGrid / 2) {
		return coef * indexGrid;
	} else if (qAbs(qAbs(coord) - (qAbs(coef) + 1) * indexGrid) <= indexGrid / 2) {
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
	const int indexGrid = SettingsManager::value("IndexGrid").toInt();
	const int coef = static_cast<int>(coord) / indexGrid;
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
	const QPointF nodeScenePos = mNode->scenePos();
	const QRectF contentsRect = mNode->contentsRect();

	// verical
	QList<QGraphicsItem *> listX = mNode->scene()->items(nodeScenePos.x(), 0
			, contentsRect.width(), widthLineY
			, Qt::IntersectsItemBoundingRect, Qt::SortOrder(), QTransform());

	// horizontal
	QList<QGraphicsItem *> listY = mNode->scene()->items(0, nodeScenePos.y()
			, widthLineX, contentsRect.height()
			, Qt::IntersectsItemBoundingRect, Qt::SortOrder(), QTransform());

	return listX + listY;
}

void SceneGridHandler::alignToGrid()
{
	if (!mSwitchGrid) {
		return;
	}
	const int indexGrid = SettingsManager::value("IndexGrid").toInt();

	const QPointF nodePos = mNode->pos();
	const QRectF contentsRect = mNode->contentsRect();

	qreal myX1 = nodePos.x() + contentsRect.x();
	qreal myY1 = nodePos.y() + contentsRect.y();

	int coefX = static_cast<int>(myX1) / indexGrid;
	int coefY = static_cast<int>(myY1) / indexGrid;

	makeGridMovingX(myX1, coefX, indexGrid);
	makeGridMovingY(myY1, coefY, indexGrid);
}

void SceneGridHandler::drawGuides()
{
	const int drift = 5;  // A dirft for scene not to resize from guide lines.
	const QPointF nodeScenePos = mNode->scenePos();
	const QRectF contentsRect = mNode->contentsRect();
	const QRectF sceneRect = mNode->scene()->sceneRect().adjusted(drift, drift, -drift, -drift);

	deleteGuides();

	QList<QGraphicsItem *> list = getAdjancedNodes();

	qreal myX1 = nodeScenePos.x() + contentsRect.x();
	qreal myY1 = nodeScenePos.y() + contentsRect.y();
	qreal myX2 = myX1 + contentsRect.width();
	qreal myY2 = myY1 + contentsRect.height();

	for (QGraphicsItem *graphicsItem : list) {
		NodeElement *item = dynamic_cast<NodeElement *>(graphicsItem);
		if (item == nullptr || item->parentItem() != nullptr || item == mNode) {
			continue;
		}

		const QPointF point = item->scenePos();
		const QRectF contents = item->contentsRect();

		const qreal pointX1 = point.x() + contents.x() - spacing;
		const qreal pointY1  = point.y() + contents.y() - spacing;
		const qreal pointX2 = pointX1  + contents.width() + 2 * spacing;
		const qreal pointY2  = pointY1 + contents.height() + 2 * spacing;

		if (pointX1 != myX1 || pointY1 != myY1) {
			const qreal deltaY1 = qAbs(pointY1 - myY1);
			const qreal deltaY2 = qAbs(pointY2 - myY2);
			const qreal deltaX1 = qAbs(pointX1 - myX1);
			const qreal deltaX2 = qAbs(pointX2 - myX2);

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
}

void SceneGridHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
	if (dynamic_cast<NodeElement*>(mNode->parentItem())) {
		return;
	}

	for (QGraphicsItem * const item : mNode->scene()->items()) {
		NodeElement * const element = dynamic_cast<NodeElement *>(item);
		if (element && element->isSelected()) {
			element->alignToGrid();
		}
	}

	drawGuides();
}
