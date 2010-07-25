#include "curve.h"
#include <QtGui/QGraphicsSceneMouseEvent>

Curve::Curve(QPointF const &start, QPointF const &end, QPointF const &c1)
	:Path(QPainterPath(start))
{
	mNeedScalingRect = false;
	mPen.setColor(Qt::gray);
	mBrush.setStyle(Qt::NoBrush);
	mDomElementType = pictureType;
	mX1 = start.x();
	mY1 = start.y();
	mX2 = end.x();
	mY2 = end.y();
	mC1 = c1;
	mCurvePath = new QPainterPath(QPointF(mX1, mY1));
	mCurvePath->quadTo(mC1, QPointF(mX2, mY2));
	mBoundingRect = boundingRect();
}

Curve::Curve(Curve const &other)
	:Path(other)
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mC1 = other.mC1;
	mCurvePath = other.mCurvePath;
	mBoundingRect = other.mBoundingRect;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Curve::clone()
{
	Curve* item = new Curve(*this);
	return item;
}

QPainterPath Curve::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPath pathCtrl1;
	pathCtrl1.moveTo(mX1, mY1);
	pathCtrl1.lineTo(mC1.x(), mC1.y());

	QPainterPath pathCtrl2;
	pathCtrl2.moveTo(mX2, mY2);
	pathCtrl2.lineTo(mC1.x(), mC1.y());

	QPainterPathStroker ps;
	ps.setWidth(drift);
	pathCtrl1 = ps.createStroke(pathCtrl1);
	pathCtrl2 = ps.createStroke(pathCtrl2);
	path = ps.createStroke(*mCurvePath);

	path.addPath(pathCtrl1);
	path.addPath(pathCtrl2);

	return path;
}

QRectF Curve::searchMaxMinCoord() const
{
	qreal minX = qMin(qMin(mX1, mX2), mC1.x());
	qreal minY = qMin(qMin(mY1, mY2), mC1.y());
	qreal maxX = qMax(qMax(mX1, mX2), mC1.x());
	qreal maxY = qMax(qMax(mY1, mY2), mC1.y());
	return QRectF(minX, minY, maxX - minX, maxY - minY);
}

QRectF Curve::boundingRect() const
{
	return searchMaxMinCoord().adjusted(-drift, -drift, drift, drift);
}

QRectF Curve::realBoundingRect() const
{
	return mapToScene(boundingRect().adjusted(drift, drift, -drift, -drift)).boundingRect();
}

void Curve::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mCurvePath = new QPainterPath(QPointF(mX1, mY1));
	mCurvePath->quadTo(mC1, QPointF(mX2, mY2));
	painter->drawPath(*mCurvePath);
}

void Curve::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(mX1, mY1);
	painter->drawPoint(mX2, mY2);
	painter->drawPoint(mC1.x(), mC1.y());

	setPenBrushDriftRect(painter);
	painter->drawPath(shape());
	drawFieldForResizeItem(painter);
}

void Curve::drawFieldForResizeItem(QPainter* painter)
{
	painter->drawEllipse(QPointF(mX1, mY1), resizeDrift, resizeDrift);
	painter->drawEllipse(QPointF(mX2, mY2), resizeDrift, resizeDrift);
	painter->drawEllipse(mC1, resizeDrift, resizeDrift);
}

void Curve::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Curve::setCXandCY(qreal x, qreal y)
{
	mC1.setX(x);
	mC1.setY(y);
	update();
}

void Curve::changeDragState(qreal x, qreal y)
{
	Item::changeDragState(x, y);
	if (QRectF(QPointF(mC1.x() + scenePos().x(), mC1.y() + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = Ctrl;
}

void  Curve::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	qreal x = mapFromScene(event->scenePos()).x();
	qreal y = mapFromScene(event->scenePos()).y();
	if (mDragState != None)
		setFlag(QGraphicsItem::ItemIsMovable, false);
	if (mDragState == TopLeft)
		setX1andY1(x, y);
	else if (mDragState == BottomRight)
		setX2andY2(x, y);
	else if (mDragState == Ctrl)
		setCXandCY(x, y);
}

QPair<QDomElement, Item::DomElementTypes> Curve::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement curve = setPenBrushToDoc(document, "curve");

	qreal const x1 = scenePos().x() + mX1 - topLeftPicture.x();
	qreal const y1 = scenePos().y() + mY1- topLeftPicture.y();
	qreal const x2 = scenePos().x() + mX2 - topLeftPicture.x();
	qreal const y2 = scenePos().y() + mY2 - topLeftPicture.y();
	qreal const x3 = scenePos().x() + mC1.x() - topLeftPicture.x();
	qreal const y3 = scenePos().y() + mC1.y() - topLeftPicture.y();

	QDomElement start  = document.createElement("start");
	curve.appendChild(start);
	start.setAttribute("starty", y1);
	start.setAttribute("startx", x1);

	QDomElement end  = document.createElement("end");
	curve.appendChild(end);
	end.setAttribute("endy", y2);
	end.setAttribute("endx", x2);

	QDomElement ctrl  = document.createElement("ctrl");
	curve.appendChild(ctrl);
	ctrl.setAttribute("y", y3);
	ctrl.setAttribute("x", x3);

	return QPair<QDomElement, Item::DomElementTypes>(curve, mDomElementType);
}
