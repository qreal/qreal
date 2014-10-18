#include "wallItem.h"

#include <QtGui/QVector2D>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <math.h>
#include <qrkernel/settingsManager.h>

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

int const wallWidth = 10;

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
	mPen.setWidth(wallWidth);
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
	// Due to screen rectangle change, mouse moves for all items on the scene. Actually
	// it doesn't happen because screen coordinates don't change.
	if (event->lastScreenPos() == event->screenPos()) {
		return;
	}

	QPointF const oldPos = pos();
	if (mDragged && ((flags() & ItemIsMovable) || mOverlappedWithRobot)) {
		QPointF const pos = event->scenePos();
		qreal const deltaX = (mX1 - mX2);
		qreal const deltaY = (mY1 - mY2);
		mX1 = pos.x() - mOldX1;
		mY1 = pos.y() - mOldY1;

		if (SettingsManager::value("2dShowGrid").toBool()) {
			int const indexGrid = SettingsManager::value("2dGridCellSize").toInt();
			reshapeBeginWithGrid(indexGrid);
			mCellNumbX1 = mX1 / indexGrid;
			mCellNumbY1 = mY1 / indexGrid;
			mCellNumbX2 = mX2 / indexGrid;
			mCellNumbY2 = mY2 / indexGrid;
		}

		setDraggedEnd(deltaX, deltaY);
	}

	 else if (mDragged) {
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
	QPainterPath wallPath;
	wallPath.moveTo(begin());
	wallPath.lineTo(end());

	QPainterPathStroker stroker;
	stroker.setWidth(wallWidth * 3 / 2);
	mPath = stroker.createStroke(wallPath);
}
