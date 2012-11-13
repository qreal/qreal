#include "line.h"
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>

const int step = 3;

using namespace graphicsUtils;

Line::Line(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent):Item(parent), mLineImpl()
{
	mNeedScalingRect = false;
	mPen.setColor(Qt::green);
	mDomElementType = pictureType;
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

Line::Line(Line const &other):Item(), mLineImpl()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Line::clone()
{
	Line* item = new Line(*this);
	return item;
}

QRectF Line::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

QRectF Line::realBoundingRect() const
{
	return mapToScene(mLineImpl.realBoundingRectWithoutScene(mX1, mY1, mX2, mY2, mPen.width(), drift)).boundingRect();
}

void Line::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mLineImpl.drawItem(painter, mX1, mY1, mX2, mY2);
}

void Line::drawExtractionForItem(QPainter* painter)
{
	mLineImpl.drawPointExtractionForItem(painter, mX1, mY1, mX2, mY2);
	setPenBrushDriftRect(painter);
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, mX1, mY1, mX2, mY2);

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
	drawScalingRects(painter);
}

void Line::drawScalingRects(QPainter* painter)
{
	QBrush brush(Qt::SolidPattern);
	if (mX2 > mX1) {
		if (mY2 > mY1) {
			brush.setColor(mListScalePoint.at(4).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 - scalingRect - step, mY1 - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(0).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 - step, mY1 - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(3).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 - scalingRect + step, mY2 + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(7).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 + step, mY2 - scalingRect + step, scalingRect, scalingRect);
		} else {
			brush.setColor(mListScalePoint.at(2).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 - step, mY1 + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(6).second);
			painter->setBrush(brush);
			painter->drawRect(mX1  - scalingRect - step, mY1 - scalingRect + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(1).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 - scalingRect + step, mY2 - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(5).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 + step, mY2 - step, scalingRect, scalingRect);
		}
	} else {
		if (mY2 > mY1) {
			brush.setColor(mListScalePoint.at(5).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 + step, mY1 - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(1).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 - scalingRect + step, mY1 - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(6).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 - scalingRect - step, mY2 - scalingRect + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(2).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 - step, mY2 + step, scalingRect, scalingRect);
		} else {
			brush.setColor(mListScalePoint.at(4).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 - scalingRect - step, mY2 - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(0).second);
			painter->setBrush(brush);
			painter->drawRect(mX2 - step, mY2 - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(3).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 - scalingRect + step, mY1 + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(7).second);
			painter->setBrush(brush);
			painter->drawRect(mX1 + step, mY1 - scalingRect + step, scalingRect, scalingRect);
		}
	}
}

QLineF Line::line() const
{
	return mLineImpl.line(mX1, mY1, mX2, mY2);
}

QPainterPath Line::shape() const
{
	return mLineImpl.shape(drift, mX1, mY1, mX2, mY2);
}

void Line::changeScalingPointState(qreal x, qreal y)
{
	qreal x1= (boundingRect().adjusted(drift, drift, -drift, -drift)).left();
	qreal x2 = (boundingRect().adjusted(drift, drift, -drift, -drift)).right();
	qreal y1 = (boundingRect().adjusted(drift, drift, -drift, -drift)).top();
	qreal y2 = (boundingRect().adjusted(drift, drift, -drift, -drift)).bottom();
	int correction = step;
	calcForChangeScalingState(QPointF(x, y), QPointF(x1, y1), QPointF(x2, y2), correction);
	if (mScalingState == topRightX || mScalingState == topRightY || mScalingState == bottomLeftX || mScalingState == bottomLeftY)
		mDragState = None;
}

void Line::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == TopLeft || mDragState == BottomRight)
		Item::resizeItem(event);
}

void Line::reshapeRectWithShift()
{
	qreal differenceX = abs(mX2 - mX1);
	qreal differenceY = abs(mY2 - mY1);
	qreal differenceXY = abs(differenceX - differenceY);
	qreal size = qMax(differenceX, differenceY);
	const int delta = size / 2;
	if (differenceXY > delta) {
		QPair<qreal, qreal> res = mLineImpl.reshapeRectWithShiftForLine(mX1, mY1, mX2, mY2, differenceX, differenceY, size);
		setX2andY2(res.first, res.second);
	} else
		Item::reshapeRectWithShift();
}

QPair<QPair<QString, QString>, QPair<QString, QString> > Line::setXandYBefore(QRect const &rect)
{
	QString x1 = "";
	QString y1 = "";
	QString y2 = "";
	QString x2 = "";
	if (mX2 > mX1) {
		if (mY2 > mY1) {
			y1 = setScaleForDoc(4, rect);
			x1 = setScaleForDoc(0, rect);
			y2 = setScaleForDoc(7, rect);
			x2 = setScaleForDoc(3, rect);
		} else {
			y1 = setScaleForDoc(6, rect);
			x1 = setScaleForDoc(2, rect);
			y2 = setScaleForDoc(5, rect);
			x2 = setScaleForDoc(1, rect);
		}
	} else {
		if (mY2 > mY1) {
			y1 = setScaleForDoc(5, rect);
			x1 = setScaleForDoc(1, rect);
			y2 = setScaleForDoc(6, rect);
			x2 = setScaleForDoc(2, rect);
		} else {
			y1 = setScaleForDoc(7, rect);
			x1 = setScaleForDoc(3, rect);
			y2 = setScaleForDoc(4, rect);
			x2 = setScaleForDoc(0, rect);
		}
	}
	return QPair<QPair<QString, QString>, QPair<QString, QString> >(QPair<QString, QString>(x1, y1), QPair<QString, QString>(x2, y2));
}

void Line::setXandY(QDomElement& dom, QPair<QPair<QString, QString>, QPair<QString, QString> > pair)
{
	dom.setAttribute("y1", pair.first.second);
	dom.setAttribute("x1", pair.first.first);
	dom.setAttribute("y2", pair.second.second);
	dom.setAttribute("x2", pair.second.first);
}

QPair<QDomElement, Item::DomElementTypes> Line::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	qreal const x1 = scenePos().x() + line().x1() - topLeftPicture.x();
	qreal const y1 = scenePos().y() + line().y1() - topLeftPicture.y();
	qreal const x2 = scenePos().x() + line().x2() - topLeftPicture.x();
	qreal const y2 = scenePos().y() + line().y2() - topLeftPicture.y();

	QDomElement line = setPenBrushToDoc(document, "line");
	setXandY(line, setXandYBefore(QRectF(x1, y1, x2 - x1, y2 - y1).normalized().toRect()));

	return QPair<QDomElement, Item::DomElementTypes>(line, mDomElementType);
}
