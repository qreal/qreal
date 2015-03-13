#include "rotater.h"

#include <QtGui/QCursor>
#include <qmath.h>

#include <qrutils/mathUtils/math.h>
#include <qrutils/graphicsUtils/rotateItem.h>

using namespace graphicsUtils;
using namespace mathUtils;

const int rotaterLength = 30;

Rotater::Rotater()
	: AbstractItem()
{
	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable, false);

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

	const QRectF rect = mMaster->rect();
	const qreal horizontalRadius = rect.width() / 2;

	mLength = rotaterLength;
	mDrift = drift > horizontalRadius ? drift / 2 : drift;
	mResizeDrift = drift > horizontalRadius ? resizeDrift / 2 : resizeDrift;


	mMaster->setFlag(ItemClipsToShape, false);
	setParentItem(mMaster);

	/// @todo Dispose of hardcoding
	mX1 = rect.right();
	// Placing rotater into the center of item`s rigth side
	mY1 = rect.y() + rect.height() / 2;
	mX2 = mX1 + mLength;
	mY2 = mY1;
}

void Rotater::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->setOpacity(0.5);
	const int addLength = mLength / 3;
	const qreal angle = addAngle;
	// Must be equal to mLength
	const qreal checkLength = sqrt((mX2 - mX1) * (mX2 - mX1) + (mY2 - mY1) * (mY2 - mY1));
	const qreal x0 = ((checkLength - addLength) * mX2 + addLength * mX1) / checkLength;
	const qreal y0 = ((checkLength - addLength) * mY2 + addLength * mY1) / checkLength;
	const QPointF first = QTransform().translate(mX2 - x0, mY2 - y0).rotate(- angle).translate(- mX2 + x0, - mY2 + y0)
			.rotate(angle).map(QPointF(x0, y0));
	const QPointF second = QTransform().translate(mX2 - x0, mY2 - y0).rotate(angle).translate(- mX2 + x0, - mY2 + y0)
			.rotate(- angle).map(QPointF(x0, y0));

	mLineImpl.drawItem(painter, mX1, mY1, mX2, mY2);
	mLineImpl.drawItem(painter, mX2, mY2, first.x(), first.y());
	mLineImpl.drawItem(painter, mX2, mY2, second.x(), second.y());
}

void Rotater::setPenBrushForExtraction(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
	Q_UNUSED(option)
	QPen pen(Qt::red);
	pen.setWidth(2);
	painter->setPen(pen);
}

void Rotater::drawExtractionForItem(QPainter *painter)
{
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, mDrift);
	drawFieldForResizeItem(painter);
}

void Rotater::drawFieldForResizeItem(QPainter *painter)
{
	painter->drawEllipse(QPointF(mX2, mY2), mResizeDrift, mResizeDrift);
}

QRectF Rotater::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

void Rotater::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	const QPointF masterCenter(mMaster->mapToScene(mMaster->rect().center()));
	const QPointF zeroRotationVector(mLength, 0);

	const qreal mouseX = event->scenePos().x() - masterCenter.x();
	const qreal mouseY = event->scenePos().y() - masterCenter.y();

	// Master rotation is signed angle between initial and mouse vector.
	// Calculating it from theese vectors product and cosine theorem
	const qreal vectorProduct = zeroRotationVector.x() * mouseY
			- zeroRotationVector.y() * mouseX;
	const qreal mouseVectorLength = sqrt(mouseX * mouseX + mouseY * mouseY);
	if (mouseVectorLength < EPS) {
		return;
	}

	const qreal translationX = mouseX - zeroRotationVector.x();
	const qreal translationY = mouseY - zeroRotationVector.y();
	const qreal translation = translationX * translationX + translationY * translationY;

	const qreal sin = vectorProduct / (mouseVectorLength * mLength);
	const bool cosIsNegative = mouseVectorLength * mouseVectorLength + mLength * mLength < translation;

	const qreal angleInWrongQuarter = asin(sin);
	const qreal angle = cosIsNegative ? M_PI - angleInWrongQuarter : angleInWrongQuarter;

	const qreal masterAngleCompensation = mMaster->parentItem()
			? mMaster->parentItem()->rotation()
			: 0.0;

	if (event->modifiers() & Qt::ShiftModifier) {
		qreal roundedAngle = (angle - fmod(angle, M_PI_4));
		if (qAbs(roundedAngle - angle) > qAbs(roundedAngle + M_PI_4 - angle)) {
			roundedAngle += M_PI_4;
		}
		mMaster->setRotation(roundedAngle * 180 / M_PI - masterAngleCompensation);
	} else {
		mMaster->setRotation(angle * 180 / M_PI - masterAngleCompensation);
	}
}

void Rotater::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == BottomRight) {
		if (mMaster->editable()) {
			AbstractItem::resizeItem(event);
		}
	}
}

void Rotater::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (mDragState != BottomRight) {
		event->ignore();
	}

	AbstractItem::mousePressEvent(event);
	mMaster->setSelected(true);
}

void Rotater::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == BottomRight) {
		if (mMaster->editable()) {
			AbstractItem::resizeItem(event);
		}

		mMaster->setSelected(true);
	}
}

void Rotater::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == BottomRight) {
		AbstractItem::mouseReleaseEvent(event);
	}
}

void Rotater::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsItem::contextMenuEvent(event);
}
