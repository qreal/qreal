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

#include "wallItem.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <qrkernel/settingsManager.h>
#include <qrutils/mathUtils/geometry.h>

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

const int wallWidth = 10;

WallItem::WallItem(const QPointF &begin, const QPointF &end)
	: SolidItem()
	, mImage(":/icons/2d_wall.png")
{
	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
	setAcceptDrops(true);
}

WallItem *WallItem::clone() const
{
	WallItem * const cloned = new WallItem({x1(), y1()}, {x2(), y2()});
	AbstractItem::copyTo(cloned);
	connect(this, &AbstractItem::positionChanged, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::x1Changed, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::y1Changed, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::x2Changed, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::y2Changed, cloned, &WallItem::recalculateBorders);

	cloned->mCellNumbX1 = mCellNumbX1;
	cloned->mCellNumbY1 = mCellNumbY1;
	cloned->mCellNumbX2 = mCellNumbX2;
	cloned->mCellNumbY2 = mCellNumbY2;

	cloned->mOldX1 = mOldX1;
	cloned->mOldY1 = mOldY1;
	cloned->mDragged = mDragged;
	cloned->mOverlappedWithRobot = mOverlappedWithRobot;
	cloned->mPath = mPath;
	return cloned;
}

QAction *WallItem::wallTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_wall.png"), tr("Wall (W)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_W));
	return result;
}

void WallItem::setPrivateData()
{
	setZValue(1);
	QPen pen(this->pen());
	pen.setWidth(wallWidth);
	pen.setStyle(Qt::NoPen);
	setPen(pen);
	QBrush brush(this->brush());
	brush.setStyle(Qt::SolidPattern);
	brush.setTextureImage(mImage);
	setBrush(brush);
}

QPointF WallItem::begin() const
{
	return QPointF(x1(), y1()) + scenePos();
}

QPointF WallItem::end() const
{
	return QPointF(x2(), y2()) + scenePos();
}

QRectF WallItem::boundingRect() const
{
	return mLineImpl.boundingRect(x1(), y1(), x2(), y2(), pen().width(), drift);
}

QPainterPath WallItem::shape() const
{
	return mLineImpl.shape(drift, x1(), y1(), x2(), y2());
}

void WallItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPath(shape());
	recalculateBorders();
}

void WallItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected()) {
		return;
	}

	painter->setPen(QPen(Qt::green));
	mLineImpl.drawExtractionForItem(painter, x1(), y1(), x2(), y2(), drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, x1(), y1(), x2(), y2());
}

void WallItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);

	if (!editable()) {
		return;
	}

	mDragged = (flags() & ItemIsMovable) || mOverlappedWithRobot;
	mOldX1 = event->scenePos().x() - x1();
	mOldY1 = event->scenePos().y() - y1();
}

void WallItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (!editable()) {
		return;
	}

	// When selecting a robot item on the scene than display widget may appear.
	// After that scene would be shrinked and mouse move event would be generated (but actually
	// mouse cursor is not moved). Because of that selected robot item moves which should not be because you
	// only select robot item and not move it. It also happens when you unselect robot item by selecting another
	// item (not only robot item). In this case new selected item moves.
	if (event->lastScreenPos() == event->screenPos()) {
		return;
	}

	const QRectF oldPos = QRectF(QPointF(x1(), y1()), QPointF(x2(), y2()));

	if (mDragged && ((flags() & ItemIsMovable) || mOverlappedWithRobot)) {
		const QPointF pos = event->scenePos();
		const qreal deltaX = (x1() - x2());
		const qreal deltaY = (y1() - y2());
		setX1(pos.x() - mOldX1);
		setY1(pos.y() - mOldY1);

		if (SettingsManager::value("2dShowGrid").toBool()) {
			const int indexGrid = SettingsManager::value("2dGridCellSize").toInt();
			reshapeBeginWithGrid(indexGrid);
			mCellNumbX1 = x1() / indexGrid;
			mCellNumbY1 = y1() / indexGrid;
			mCellNumbX2 = x2() / indexGrid;
			mCellNumbY2 = y2() / indexGrid;
		}

		setDraggedEnd(deltaX, deltaY);
	}  else if (mDragged) {
		QGraphicsItem::mouseMoveEvent(event);
	}

	// Items under cursor cannot be dragged when adding new item,
	// but it mustn`t confuse the case when item is unmovable
	// because overapped with robot
	if (mDragged && ((flags() & ItemIsMovable) || mOverlappedWithRobot)) {
		emit wallDragged(this, realShape(), oldPos);
	}
	event->accept();
}

bool WallItem::isDragged() const
{
	return mDragged;
}

qreal WallItem::width() const
{
	return pen().width();
}

void WallItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mDragged = false;
}

void WallItem::serialize(QDomElement &wallNode) const
{
	wallNode.setTagName("wall");
	AbstractItem::serialize(wallNode);
	mLineImpl.serialize(wallNode, x1() + scenePos().x(), y1() + scenePos().y()
			, x2() + scenePos().x(), y2() + scenePos().y());
}

void WallItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	const QPair<QPointF, QPointF> points = mLineImpl.deserialize(element);
	const QPointF begin = points.first;
	const QPointF end = points.second;

	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());

	recalculateBorders();
}

void WallItem::onOverlappedWithRobot(bool overlapped)
{
	mOverlappedWithRobot = overlapped;
}

QPainterPath WallItem::path() const
{
	return mPath;
}

void WallItem::recalculateBorders()
{
	QPainterPath wallPath;
	wallPath.moveTo(begin());

	if (mathUtils::Geometry::eq(begin(), end())) {
		wallPath.lineTo(end().x() + 0.1, end().y());
	} else {
		wallPath.lineTo(end());
	}

	QPainterPathStroker stroker;
	stroker.setWidth(wallWidth * 3 / 2);
	mPath = stroker.createStroke(wallPath);
}

void WallItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (event->modifiers() & Qt::ShiftModifier && (dragState() == TopLeft || dragState() == BottomRight)) {
		AbstractItem::resizeItem(event);
		reshapeRectWithShift();
	} else {
		if (SettingsManager::value("2dShowGrid").toBool() && (dragState() == TopLeft || dragState() == BottomRight)) {
			resizeWithGrid(event, SettingsManager::value("2dGridCellSize").toInt());
		} else {
			if (dragState() == TopLeft || dragState() == BottomRight) {
				SolidItem::resizeItem(event);
			} else {
				setFlag(QGraphicsItem::ItemIsMovable, true);
			}
		}
	}
}

void WallItem::reshapeRectWithShift()
{
	const qreal differenceX = qAbs(x2() - x1());
	const qreal differenceY = qAbs(y2() - y1());
	const qreal differenceXY = qAbs(differenceX - differenceY);
	const qreal size = qMax(differenceX, differenceY);
	const int delta = size / 2;
	if (differenceXY > delta) {
		const qreal corner1X = dragState() == TopLeft ? x2() : x1();
		const qreal corner1Y = dragState() == TopLeft ? y2() : y1();
		const qreal corner2X = dragState() == TopLeft ? x1() : x2();
		const qreal corner2Y = dragState() == TopLeft ? y1() : y2();
		const QPair<qreal, qreal> res = mLineImpl.reshapeRectWithShiftForLine(corner1X, corner1Y, corner2X, corner2Y
				, differenceX, differenceY, size);
		if (dragState() == TopLeft) {
			setX1(res.first);
			setY1(res.second);
		} else {
			setX2(res.first);
			setY2(res.second);
		}
	} else {
		const qreal size = qMax(qAbs(x2() - x1()), qAbs(y2() - y1()));
		if (dragState() == TopLeft) {
			setX1(x1() > x2() ? x2() + size : x2() - size);
			setY1(y1() > y2() ? y2() + size : y2() - size);
		} else {
			setX2(x2() > x1() ? x1() + size : x1() - size);
			setY2(y2() > y1() ? y1() + size : y1() - size);
		}
	}
}

void WallItem::resizeWithGrid(QGraphicsSceneMouseEvent *event, int indexGrid)
{
	const qreal x = mapFromScene(event->scenePos()).x();
	const qreal y = mapFromScene(event->scenePos()).y();

	if (dragState() != None) {
		setFlag(QGraphicsItem::ItemIsMovable, false);
	}

	if (dragState() == TopLeft) {
		setX1(x);
		setY1(y);
		resizeBeginWithGrid(indexGrid);
	} else if (dragState() == BottomRight) {
		setX2(x);
		setY2(y);
		reshapeEndWithGrid(indexGrid);
	}
}

void WallItem::resizeBeginWithGrid(int indexGrid)
{
	const int coefX = static_cast<int>(x1()) / indexGrid;
	const int coefY = static_cast<int>(y1()) / indexGrid;

	setX1(alignedCoordinate(x1(), coefX, indexGrid));
	setY1(alignedCoordinate(y1(), coefY, indexGrid));

	mCellNumbX1 = x1() / indexGrid;
	mCellNumbY1 = y1() / indexGrid;
}

void WallItem::reshapeEndWithGrid(int indexGrid)
{
	const int coefX = static_cast<int>(x2()) / indexGrid;
	const int coefY = static_cast<int>(y2()) / indexGrid;

	setX2(alignedCoordinate(x2(), coefX, indexGrid));
	setY2(alignedCoordinate(y2(), coefY, indexGrid));

	mCellNumbX2 = x2() / indexGrid;
	mCellNumbY2 = y2() / indexGrid;
}

void WallItem::reshapeBeginWithGrid(int indexGrid)
{
	const int coefX = static_cast<int> (x1()) / indexGrid;
	const int coefY = static_cast<int> (y1()) / indexGrid;
	setX1(alignedCoordinate(x1(), coefX, indexGrid));
	setY1(alignedCoordinate(y1(), coefY, indexGrid));
	mCellNumbX1 = x1() / indexGrid;
	mCellNumbY1 = y1() / indexGrid;
}

void WallItem::alignTheWall(int indexGrid)
{
	countCellNumbCoordinates(indexGrid);
	setBeginCoordinatesWithGrid(indexGrid);
	setEndCoordinatesWithGrid(indexGrid);
}

void WallItem::countCellNumbCoordinates(int indexGrid)
{
	mCellNumbX1 = x1() / indexGrid;
	mCellNumbY1 = y1() / indexGrid;

	if (qAbs(y2() - y1()) > qAbs(x2() - x1())) {
		mCellNumbX2 = mCellNumbX1;
		mCellNumbY2 = y2() / indexGrid;
	} else {
		mCellNumbX2 = x2() / indexGrid;
		mCellNumbY2 = mCellNumbY1;
	}
}

void WallItem::setBeginCoordinatesWithGrid(int indexGrid)
{
	setX1(mCellNumbX1 * indexGrid);
	setY1(mCellNumbY1 * indexGrid);
}

void WallItem::setEndCoordinatesWithGrid(int indexGrid)
{
	setX2(mCellNumbX2 * indexGrid);
	setY2(mCellNumbY2 * indexGrid);
}

void WallItem::setDraggedEnd(qreal x, qreal y)
{
	setX2(x1() - x);
	setY2(y1() - y);
}

qreal WallItem::alignedCoordinate(qreal coord, int coef, const int indexGrid) const
{
	const int coefSign = coef ? coef / qAbs(coef) : 0;

	if (qAbs(qAbs(coord) - qAbs(coef) * indexGrid) <= indexGrid / 2) {
		return coef * indexGrid;
	} else if (qAbs(qAbs(coord) - (qAbs(coef) + 1) * indexGrid) <= indexGrid / 2) {
		return (coef + coefSign) * indexGrid;
	}

	return coord;
}
