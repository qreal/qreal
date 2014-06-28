#include "wallItem.h"

#include <QtGui/QVector2D>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <math.h>
#include <qrkernel/settingsManager.h>

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

WallItem::WallItem(QPointF const &begin, QPointF const &end)
	: LineItem(begin, end)
	, mDragged(false)
	, mImage(":/icons/2d_wall.png")
	, mOldX1(0)
	, mOldY1(0)
{
	setPrivateData();
	setAcceptDrops(true);
}

void WallItem::setPrivateData()
{
	setZValue(1);
	mPen.setWidth(10);
	mPen.setStyle(Qt::NoPen);
	mBrush.setStyle(Qt::SolidPattern);
	mBrush.setTextureImage(mImage);
	mSerializeName = "wall";
}

QPointF WallItem::begin()
{
	return QPointF(mX1, mY1) + scenePos();
}

QPointF WallItem::end()
{
	return QPointF(mX2, mY2) + scenePos();
}

void WallItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, mX1, mY1, mX2, mY2);
}

void WallItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mDragged = (flags() & ItemIsMovable) || mOverlappedWithRobot;
	mOldX1 = event->scenePos().x() - mX1;
	mOldY1 = event->scenePos().y() - mY1;
}

void WallItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF const oldPos = pos();
	if (SettingsManager::value("2dShowGrid").toBool() && mDragged
			&& ((flags() & ItemIsMovable) || mOverlappedWithRobot)) {
		QPointF const pos = event->scenePos();
		int const indexGrid = SettingsManager::value("2dGridCellSize").toInt();
		qreal const deltaX = (mX1 - mX2);
		qreal const deltaY = (mY1 - mY2);
		mX1 = pos.x() - mOldX1;
		mY1 = pos.y() - mOldY1;
		reshapeBeginWithGrid(indexGrid);
		setDraggedEndWithGrid(deltaX, deltaY);
		mCellNumbX1 = mX1 / indexGrid;
		mCellNumbY1 = mY1 / indexGrid;
		mCellNumbX2 = mX2 / indexGrid;
		mCellNumbY2 = mY2 / indexGrid;
	} else if (mDragged) {
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
	return mPen.width();
}

void WallItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mDragged = false;
}

QDomElement WallItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement wallNode = document.createElement(mSerializeName);
	wallNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
	wallNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
	return wallNode;
}

void WallItem::deserializePenBrush(QDomElement const &element)
{
	Q_UNUSED(element)
	setPrivateData();
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
	qreal const x1 = begin().x();
	qreal const x2 = end().x();
	qreal const y1 = begin().y();
	qreal const y2 = end().y();

	qreal dx = x2 - x1;
	qreal dy = y2 - y1;
	qreal const len = sqrt(dx * dx + dy * dy);
	dx /= len;
	dy /= len;
	dx *= 5;
	dy *= 5;

	QVector2D norm(y1 - y2, x2 - x1);
	norm.normalize();
	norm *= mPen.widthF() / 2;

	QPointF const point1(x1 - dx + norm.x(), y1 - dy + norm.y());
	QPointF const point2(x1 - dx - norm.x(), y1 - dy - norm.y());
	QPointF const point3(x2 + dx - norm.x(), y2 + dy - norm.y());
	QPointF const point4(x2 + dx + norm.x(), y2 + dy + norm.y());

	QPainterPath wallPath;
	wallPath.moveTo(point1);
	wallPath.lineTo(point2);
	wallPath.lineTo(point3);
	wallPath.lineTo(point4);
	wallPath.lineTo(point1);

	mPath = wallPath;
}
