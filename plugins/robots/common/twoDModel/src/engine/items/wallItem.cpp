#include "wallItem.h"

#include <QtGui/QVector2D>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <math.h>
#include <qrkernel/settingsManager.h>

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

const int wallWidth = 10;

WallItem::WallItem(const QPointF &begin, const QPointF &end)
	: LineItem(begin, end)
	, mDragged(false)
	, mImage(":/icons/2d_wall.png")
	, mOldX1(0)
	, mOldY1(0)
{
	setPrivateData();
	setAcceptDrops(true);
}

AbstractItem *WallItem::clone() const
{
	const auto cloned = new WallItem({x1(), y1()}, {x2(), y2()});
	AbstractItem::copyTo(cloned);
	connect(this, &AbstractItem::positionChanged, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::x1Changed, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::y1Changed, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::x2Changed, cloned, &WallItem::recalculateBorders);
	connect(this, &AbstractItem::y2Changed, cloned, &WallItem::recalculateBorders);
	cloned->mOldX1 = mOldX1;
	cloned->mOldY1 = mOldY1;
	cloned->mDragged = mDragged;
	cloned->mOverlappedWithRobot = mOverlappedWithRobot;
	cloned->mPath = mPath;
	return cloned;
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
	mSerializeName = "wall";
}

QPointF WallItem::begin()
{
	return QPointF(x1(), y1()) + scenePos();
}

QPointF WallItem::end()
{
	return QPointF(x2(), y2()) + scenePos();
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

	const QRectF oldPos =  QRectF(QPointF(x1(), y1()), QPointF(x2(), y2()));

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

void WallItem::deserialize(const QDomElement &element)
{
	LineItem::deserialize(element);
	recalculateBorders();
}

QDomElement WallItem::serialize(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement wallNode = document.createElement(mSerializeName);
	AbstractItem::serialize(wallNode);
	wallNode.setAttribute("begin", QString::number(x1() + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(y1() + scenePos().y() - topLeftPicture.y()));
	wallNode.setAttribute("end", QString::number(x2() + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(y2() + scenePos().y() - topLeftPicture.y()));
	return wallNode;
}

void WallItem::deserializePenBrush(const QDomElement &element)
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
