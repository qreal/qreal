#include "ejectedItem.h"
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

int const currentResizeDrift = resizeDrift / 2;
int const currentDrift = drift / 2;

EjectedItem::EjectedItem(QPointF begin, QPointF end)
	: mRectImpl()
{
	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
}

void EjectedItem::setPrivateData()
{
	mPen.setColor(Qt::green);
	mPen.setStyle(Qt::SolidLine);
	mBrush.setColor(Qt::blue);
	mBrush.setStyle(Qt::SolidPattern);
	mSerializeName = "ejectedItem";
}

QRectF EjectedItem::calcNecessaryBoundingRect() const
{
	return QRectF(qMin(mX1, mX2), qMin(mY1, mY2), abs(mX2 - mX1), abs(mY2 - mY1));
}

QRectF EjectedItem::boundingRect() const
{
	return mRectImpl.boundingRect(mX1, mY1, mX2, mY2, currentDrift);
}

void EjectedItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mRectImpl.drawRectItem(painter, mX1, mY1, mX2, mY2);
}

void EjectedItem::drawFieldForResizeItem(QPainter* painter)
{
	AbstractItem::drawFieldForResizeItem(painter, currentResizeDrift);
}

QDomElement EjectedItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement rectNode = setPenBrushToDoc(document, mSerializeName);
	rectNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x())
						 + ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
	rectNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x())
						 + ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
	return rectNode;
}

void EjectedItem::deserialize(QDomElement const &element)
{
	QString const beginStr = element.attribute("begin", "0:0");
	QStringList splittedStr = beginStr.split(":");
	int x = splittedStr[0].toInt();
	int y = splittedStr[1].toInt();
	QPointF const begin = QPointF(x, y);

	QString const endStr = element.attribute("end", "0:0");
	splittedStr = endStr.split(":");
	x = splittedStr[0].toInt();
	y = splittedStr[1].toInt();
	QPointF const end = QPointF(x, y);

	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();
	readPenBrush(element);
}

void EjectedItem::robotChangedPosition(QRectF const &newRect, QPointF const& diffPos)
{
	if (realBoundingRect().intersects(newRect)) {
		mMoved = true;
		QPointF oldPos = pos();
		setPos(pos() + diffPos);
		emit ejectedItemMoved(realBoundingRect(), oldPos, diffPos);
	} else {
		mMoved = false;
	}
}

void EjectedItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mDragged = true;
}

void EjectedItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF oldPos = pos();
	QGraphicsItem::mouseMoveEvent(event);
	if (mDragged) {
		emit ejectedItemDragged(realBoundingRect(), oldPos);
	}
}

void EjectedItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mDragged = false;
}

bool EjectedItem::isDragged()
{
	return mDragged;
}

void EjectedItem::toStopDraggedEjectedItem(bool isNeedStop, QPointF const& oldPos)
{
	if (mDragged) {
		toStopEjectedItem(isNeedStop, oldPos);
	}
}

void EjectedItem::toStopMovedEjectedItem(bool isNeedStop, QPointF const& oldPos)
{
	if (mMoved) {
		toStopEjectedItem(isNeedStop, oldPos);
	}
}

void EjectedItem::toStopEjectedItem(bool isNeedStop, QPointF const& oldPos)
{
	if (isNeedStop) {
		setPos(oldPos);
		setFlag(ItemIsMovable, false);
	} else {
		setFlag(ItemIsMovable, true);
	}
}
