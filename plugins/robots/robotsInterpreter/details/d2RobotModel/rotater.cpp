#include <QtWidgets/QCursor>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <qmath.h>

#include "rotater.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

Rotater::Rotater() : AbstractItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(2);

	mPen.setColor(Qt::blue);
	mPen.setWidth(3);
	mBrush.setStyle(Qt::NoBrush);
}

void Rotater::setMasterItem(RotateItem *masterItem)
{
	mMaster = masterItem;

	mLength = 30; //mMaster->horizontalRadius();//asd

	QRectF const rect = mMaster->rect();

	mMaster->setFlag(ItemClipsToShape, false);
	setParentItem(mMaster);

	// TODO: Dispose of hardcoding
	mX1 = rect.width();
	// Placing rotater into the center of item`s rigth side
	mY1 = rect.height() / 2 - 5;
	mX2 = mX1 + mLength;
	mY2 = mY1;
}

void Rotater::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->setOpacity(0.5);
	const int addLength = mLength / 3;
	qreal angle = addAngle;
	// Must be equal to mLength
	qreal checkLength = sqrt((mX2 - mX1) * (mX2 - mX1) + (mY2 - mY1) * (mY2 - mY1));
	qreal x0 = ((checkLength - addLength) * mX2 + addLength * mX1) / checkLength;
	qreal y0 = ((checkLength - addLength) * mY2 + addLength * mY1) / checkLength;
	QPointF first = QTransform().translate(mX2 - x0, mY2 - y0).rotate(- angle).translate(- mX2 + x0, - mY2 + y0).rotate(angle).map(QPointF(x0, y0));
	QPointF second = QTransform().translate(mX2 - x0, mY2 - y0).rotate(angle).translate(- mX2 + x0, - mY2 + y0).rotate(- angle).map(QPointF(x0, y0));

	mLineImpl.drawItem(painter, mX1, mY1, mX2, mY2);
	mLineImpl.drawItem(painter, mX2, mY2, first.x(), first.y());
	mLineImpl.drawItem(painter, mX2, mY2, second.x(), second.y());
}

void Rotater::setPenBrushForExtraxtion(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
	Q_UNUSED(option)
	QPen pen(Qt::red);
	pen.setWidth(2);
	painter->setPen(pen);
}

void Rotater::drawExtractionForItem(QPainter* painter)
{
	int driftForRotater = drift;
	if (drift > mMaster->horizontalRadius()) {
		driftForRotater = drift / 2;
	}
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, driftForRotater);
	drawFieldForResizeItem(painter);
}

void Rotater::drawFieldForResizeItem(QPainter* painter)
{
	int resizeDriftForRotater = resizeDrift;
	if (drift > mMaster->horizontalRadius()) {
		resizeDriftForRotater = resizeDrift / 2;
	}
	painter->drawEllipse(QPointF(mX2, mY2), resizeDriftForRotater, resizeDriftForRotater);
}

QRectF Rotater::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

void Rotater::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	// Cosine theorem
	qreal const x1 = event->pos().x();
	qreal const y1 = event->pos().y();
	qreal const x2 = event->lastPos().x();
	qreal const y2 = event->lastPos().y();
	qreal len = sqrt((x1 * x1 + y1 * y1) * (x2 * x2 + y2 * y2));

	// Rotation sign is the sign of the vector product
	qreal const vectorProduct = x1 * y2 - x2 * y1;
	int const sign = vectorProduct < 0 ? -1 : 1;

	qreal const eps = 10e-8;
	qreal const dalpha = len < eps ? 0 : acos((x1 * x2 + y1 * y2) / len);
	mMaster->rotate(mMaster->rotation() - sign * dalpha * 180 / M_PI);
}

void Rotater::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == BottomRight) {
		setFlag(ItemIsMovable, false);
		AbstractItem::resizeItem(event);
	}
}

void Rotater::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mMaster->setSelected(true);
}

void Rotater::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseMoveEvent(event);
	mMaster->setSelected(true);
}

void Rotater::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
}
