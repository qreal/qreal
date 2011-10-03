#include "rotater.h"

#include <QtGui/QCursor>
#include <QtGui/QStyleOptionGraphicsItem>
#include <qmath.h>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

Rotater::Rotater() : AbstractItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(2);

	mPen.setColor(Qt::blue);
	mPen.setWidth(3);
	mBrush.setStyle(Qt::NoBrush);
}

void Rotater::setMasterItem(RotateInterface *masterItem)
{
	mMaster = masterItem;

	mLength = 30; //mMaster->horizontalRadius();//asd

	QRectF rect = mMaster->rect();
	mX1 = rect.topRight().x();
	mY1 = rect.topRight().y() + rect.height() / 2;//asd плохо, если делать другую точку поворота

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
	qreal checkLength = sqrt((mX2 - mX1) * (mX2 - mX1) + (mY2 - mY1) * (mY2 - mY1));// должен быть равен mLength
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
	if(drift > mMaster->horizontalRadius())
			driftForRotater = drift / 2;
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, driftForRotater);
	drawFieldForResizeItem(painter);
}

void Rotater::drawFieldForResizeItem(QPainter* painter)
{
	int resizeDriftForRotater = resizeDrift;
	if(drift > mMaster->horizontalRadius())
			resizeDriftForRotater = resizeDrift / 2;
	painter->drawEllipse(QPointF(mX2, mY2), resizeDriftForRotater, resizeDriftForRotater);
}

QRectF Rotater::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

void Rotater::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	qreal x = mapFromScene(event->scenePos()).x();
	qreal y = mapFromScene(event->scenePos()).y();

	qreal realLength = sqrt((x - mMasterRotatePoint.x()) * (x - mMasterRotatePoint.x()) + (y - mMasterRotatePoint.y()) * (y - mMasterRotatePoint.y()));
	qreal newX1 = (mMasterRotatePoint.x() * (realLength - mMasterSize) + x * mMasterSize) / realLength;
	qreal newY1 = (mMasterRotatePoint.y() * (realLength - mMasterSize) + y * mMasterSize) / realLength;
	qreal newX2 = (mMasterRotatePoint.x() * (realLength - mMasterSize - mLength) + x * (mMasterSize + mLength)) / realLength;
	qreal newY2 = (mMasterRotatePoint.y() * (realLength - mMasterSize - mLength) + y * (mMasterSize + mLength)) / realLength;

	setX1andY1(newX1, newY1);
	setX2andY2(newX2, newY2);
}

void Rotater::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == BottomRight) {

		mMasterSize = mMaster->horizontalRadius();
		qreal oldX1 = mX1;
		qreal oldY1 = mY1;
		mMasterRotatePoint.setX(((mMasterSize + mLength) * mX1 - mMasterSize * mX2) / mLength);
		mMasterRotatePoint.setY(((mMasterSize + mLength) * mY1 - mMasterSize * mY2) / mLength);

		AbstractItem::resizeItem(event);

		qreal tempLength = sqrt((mX1 - oldX1) * (mX1 - oldX1) + (mY1 - oldY1) * (mY1 - oldY1));
		double angle = 2 * asin(tempLength / (2 * mMasterSize)) * 180 / M_PI;

		qreal difX1 = oldX1 - mX1;
		qreal difY1 = oldY1 - mY1;
		if (mMasterRotatePoint.x() < oldX1 && mMasterRotatePoint.x() > mX1) {
			if (mMasterRotatePoint.y() >= oldY1 && mMasterRotatePoint.y() >= mY1) {
				angle = - angle;
			}
		} else if (mMasterRotatePoint.x() > oldX1 && mMasterRotatePoint.x() < mX1) {
			if (mMasterRotatePoint.y() <= oldY1 && mMasterRotatePoint.y() <= mY1) {
				angle = - angle;
			}
		} else if (mMasterRotatePoint.y() < oldY1 && mMasterRotatePoint.y() > mY1) {
			if (mMasterRotatePoint.x() <= oldX1 && mMasterRotatePoint.x() <= mX1) {
				angle = - angle;
			}
		} else if (mMasterRotatePoint.y() > oldY1 && mMasterRotatePoint.y() < mY1) {
			if (mMasterRotatePoint.x() >= oldX1 && mMasterRotatePoint.x() >= mX1) {
				angle = - angle;
			}
		} else if (mMasterRotatePoint.y()  <= mY1) {
			if (difX1 < 0 && difY1 > 0) {
				angle = - angle;
			} else if (difX1 < 0 && difY1 < 0) {
				angle = - angle;
			} else if (difY1 == 0 && oldX1 < mX1) {
				angle = - angle;
			}
		} else if(mMasterRotatePoint.y()  >= oldY1) {
			if (difX1 > 0 && difY1 > 0) {
				angle = - angle;
			} else if (difX1 > 0 && difY1 < 0) {
				angle = - angle;
			}  else if (difY1 == 0 && oldX1 > mX1) {
				angle = - angle;
			}
		}
		mMaster->rotate(angle);
	}
}

void Rotater::reshapeWithMasterItem(QPointF delta)
{
//	if (!isSelected())//надо бы уточнить...
		setPos(pos() + delta);
}

void Rotater::rotateWithMasterItem(QPointF delta, QPointF rotatePoint, QPointF basePos, qreal baseDir, qreal localDir)
{
	delta = QPointF(0, 0);
	QRectF rect = mMaster->rect();
	qreal x1 = rect.topRight().x() + delta.x();
	qreal y1 = rect.topRight().y() + rect.height() / 2 + delta.y();//asd плохо, если делать другую точку поворота
	qreal x2 = x1 + mLength;
	qreal y2 = y1;

	QPointF point1 = QPointF(x1, y1) - basePos - rotatePoint;
	QPointF firstPoint = QTransform().translate(-point1.x(), -point1.y()).rotate(baseDir)
						.translate(point1.x(), point1.y()).rotate(-baseDir)
						.map(QPointF(x1, y1) - basePos);
	mX1 = firstPoint.x() + basePos.x();
	mY1 = firstPoint.y() + basePos.y();

	QPointF point2 = QPointF(x2, y2) - basePos - rotatePoint;
	QPointF secondPoint = QTransform().translate(-point2.x(), -point2.y()).rotate(baseDir)
						.translate(point2.x(), point2.y()).rotate(-baseDir)
						.map(QPointF(x2, y2) - basePos);
	mX2 = secondPoint.x() + basePos.x();
	mY2 = secondPoint.y() + basePos.y();

	QPointF resSecondPoint = QTransform().translate(-(mX2 - mX1), -(mY2 - mY1)).rotate(localDir)
						.translate((mX2 - mX1), (mY2 - mY1)).rotate(-localDir)
						.map(QPointF(mX2, mY2));

	mX2 = resSecondPoint.x();
	mY2 = resSecondPoint.y();
}

void Rotater::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mMaster->setSelected(true);
	setFlag(ItemIsMovable, false);
}

void Rotater::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseMoveEvent(event);
	mMaster->setSelected(true);
	setFlag(ItemIsMovable, false);
}

void Rotater::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
	setFlag(ItemIsMovable, false);
}

