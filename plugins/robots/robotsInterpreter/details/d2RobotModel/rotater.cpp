#include "rotater.h"

#include <QtGui/QCursor>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <qmath.h>

#include <qrutils/mathUtils/math.h>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;
using namespace mathUtils;

Rotater::Rotater() : AbstractItem()
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

	mLength = 30; //mMaster->horizontalRadius();//asd
	mDrift = drift > mMaster->horizontalRadius() ? drift / 2 : drift;
	mResizeDrift = drift > mMaster->horizontalRadius() ? resizeDrift / 2 : resizeDrift;

	QRectF const rect = mMaster->rect();

	mMaster->setFlag(ItemClipsToShape, false);
	setParentItem(mMaster);

	// TODO: Dispose of hardcoding
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
	qreal const angle = addAngle;
	// Must be equal to mLength
	qreal const checkLength = sqrt((mX2 - mX1) * (mX2 - mX1) + (mY2 - mY1) * (mY2 - mY1));
	qreal const x0 = ((checkLength - addLength) * mX2 + addLength * mX1) / checkLength;
	qreal const y0 = ((checkLength - addLength) * mY2 + addLength * mY1) / checkLength;
	QPointF const first = QTransform().translate(mX2 - x0, mY2 - y0).rotate(- angle).translate(- mX2 + x0, - mY2 + y0).rotate(angle).map(QPointF(x0, y0));
	QPointF const second = QTransform().translate(mX2 - x0, mY2 - y0).rotate(angle).translate(- mX2 + x0, - mY2 + y0).rotate(- angle).map(QPointF(x0, y0));

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
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, mDrift);
	drawFieldForResizeItem(painter);
}

void Rotater::drawFieldForResizeItem(QPainter* painter)
{
	painter->drawEllipse(QPointF(mX2, mY2), mResizeDrift, mResizeDrift);
}

QRectF Rotater::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

void Rotater::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	QPointF const masterCenter(mMaster->mapToScene(mMaster->rect().center()));
	QPointF const zeroRotationVector(mLength, 0);

	qreal const mouseX = event->scenePos().x() - masterCenter.x();
	qreal const mouseY = event->scenePos().y() - masterCenter.y();

	// Master rotation is signed angle between initial and mouse vector.
	// Calculating it from theese vectors product and cosine theorem
	qreal const vectorProduct = zeroRotationVector.x() * mouseY
			- zeroRotationVector.y() * mouseX;
	qreal const mouseVectorLength = sqrt(mouseX * mouseX + mouseY * mouseY);
	if (mouseVectorLength < EPS) {
		return;
	}

	qreal const translationX = mouseX - zeroRotationVector.x();
	qreal const translationY = mouseY - zeroRotationVector.y();
	qreal const translation = translationX * translationX + translationY * translationY;

	qreal const sin = vectorProduct / (mouseVectorLength * mLength);
	bool const cosIsNegative = mouseVectorLength * mouseVectorLength + mLength * mLength < translation;

	qreal const angleInWrongQuarter = asin(sin);
	qreal const angle = cosIsNegative ? M_PI - angleInWrongQuarter : angleInWrongQuarter;

	qreal const masterAngleCompensation = mMaster->parentItem()
			? mMaster->parentItem()->rotation()
			: 0.0;

	if (event->modifiers() & Qt::ShiftModifier) {
		qreal roundedAngle = (angle - fmod(angle, M_PI_4));
		if (qAbs(roundedAngle - angle) > qAbs(roundedAngle + M_PI_4 - angle)) {
			roundedAngle += M_PI_4;
		}
		mMaster->rotate(roundedAngle * 180 / M_PI - masterAngleCompensation);
	} else {
		mMaster->rotate(angle * 180 / M_PI - masterAngleCompensation);
	}
}

void Rotater::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == BottomRight) {
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
	if (mDragState == BottomRight) {
		AbstractItem::resizeItem(event);
		mMaster->setSelected(true);
	}
}

void Rotater::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (mDragState == BottomRight) {
		AbstractItem::mouseReleaseEvent(event);
	}
}
