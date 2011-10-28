#include "abstractItem.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QGraphicsScene>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QDebug>

using namespace graphicsUtils;

AbstractItem::AbstractItem(QGraphicsItem* parent)
	: QGraphicsItem(parent), mDragState(None)
	, mX1(0), mY1(0), mX2(0), mY2(0), mView(NULL)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	mBrush.setColor(mPen.color());
}

QRectF AbstractItem::calcNecessaryBoundingRect() const
{
	return boundingRect();
}

QRectF AbstractItem::realBoundingRect() const
{
	return mapToScene(calcNecessaryBoundingRect()).boundingRect();
}

void AbstractItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	painter->setPen(mPen);
	painter->setBrush(mBrush);
	drawItem(painter, option, widget);
	if (option->state & QStyle::State_Selected) {
		painter->save();
		setPenBrushForExtraxtion(painter, option);
		drawExtractionForItem(painter);
		painter->restore();
	}
}

void AbstractItem::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(mX1, mY1);
	painter->drawPoint(mX1, mY2);
	painter->drawPoint(mX2, mY1);
	painter->drawPoint(mX2, mY2);
	drawFieldForResizeItem(painter);
}

void AbstractItem::drawFieldForResizeItem(QPainter* painter)
{
	QRectF itemBoundingRect = calcNecessaryBoundingRect();
	qreal x1 = itemBoundingRect.left();
	qreal x2 = itemBoundingRect.right();
	qreal y1 = itemBoundingRect.top();
	qreal y2 = itemBoundingRect.bottom();

	setPenBrushDriftRect(painter);
	painter->drawRect(x1, y1, resizeDrift, resizeDrift);
	painter->drawRect(x2 - resizeDrift, y2 - resizeDrift, resizeDrift, resizeDrift);
	painter->drawRect(x1, y2 - resizeDrift, resizeDrift, resizeDrift);
	painter->drawRect(x2 - resizeDrift, y1, resizeDrift, resizeDrift);
}

void AbstractItem::setPenBrushForExtraxtion(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
	Q_UNUSED(option);
	QPen pen(Qt::red);
	pen.setWidth(3);
	painter->setPen(pen);
}

void AbstractItem::setPenBrushDriftRect(QPainter* painter)
{
	QPen pen(QColor("whitesmoke"));
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(0);
	QBrush brush;
	brush.setStyle(Qt::NoBrush);
	brush.setColor(Qt::white);
	painter->setPen(pen);
	painter->setBrush(brush);
}

QPen AbstractItem::pen() const
{
	return mPen;
}

QBrush AbstractItem::brush() const
{
	return mBrush;
}

void AbstractItem::setBrush(QBrush const &brush)
{
	mBrush = brush;
}

void AbstractItem::setPen(QPen const &pen)
{
	mPen = pen;
}

void AbstractItem::setX1andY1(qreal x, qreal y)
{
	mX1 = x;
	mY1 = y;
	update();
}

void AbstractItem::setX1andY2(qreal x, qreal y)
{
	mX1 = x;
	mY2 = y;
	update();
}

void AbstractItem::setX2andY1(qreal x, qreal y)
{
	mX2 = x;
	mY1 = y;
	update();
}

void AbstractItem::setX2andY2(qreal x, qreal y)
{
	mX2 = x;
	mY2 = y;
	update();
}

void AbstractItem::reshapeRectWithShift()
{
	qreal size = qMax(abs(mX2 - mX1), abs(mY2 - mY1));
	if(mX2 > mX1) {
		if (mY2 > mY1)
			setX2andY2(mX1 + size, mY1 + size);
		else
			setX2andY2(mX1 + size, mY1 - size);
	} else {
		if (mY2 > mY1)
			setX2andY2(mX1 - size, mY1 + size);
		else
			setX2andY2(mX1 - size, mY1 - size);
	}
}

void AbstractItem::changeDragState(qreal x, qreal y)
{
	if (QRectF(QPointF(mX1 + scenePos().x(), mY1 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = TopLeft;
	else if (QRectF(QPointF(mX2 + scenePos().x(), mY2 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = BottomRight;
	else if (QRectF(QPointF(mX2 + scenePos().x(), mY1 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = TopRight;
	else if (QRectF(QPointF(mX1 + scenePos().x(), mY2 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = BottomLeft;
	else
		mDragState = None;
}

AbstractItem::DragState AbstractItem::getDragState() const
{
	return mDragState;
}

void AbstractItem::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	qreal x = mapFromScene(event->scenePos()).x();
	qreal y = mapFromScene(event->scenePos()).y();
	if (mDragState != None)
		setFlag(QGraphicsItem::ItemIsMovable, false);
	if (mDragState == TopLeft)
		setX1andY1(x, y);
	else if (mDragState == TopRight)
		setX2andY1(x, y);
	else if (mDragState == BottomLeft)
		setX1andY2(x, y);
	else if (mDragState == BottomRight)
		setX2andY2(x, y);
}

void AbstractItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState != None)
		calcResizeItem(event);
	else {
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void AbstractItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
}

void AbstractItem::setPenStyle(const QString& text)
{
	if (text == "Solid")
		mPen.setStyle(Qt::SolidLine);
	else if (text == "Dot")
		mPen.setStyle(Qt::DotLine);
	else if (text == "Dash")
		mPen.setStyle(Qt::DashLine);
	else if (text == "DashDot")
		mPen.setStyle(Qt::DashDotLine);
	else if (text == "DashDotDot")
		mPen.setStyle(Qt::DashDotDotLine);
	else if (text == "None")
		mPen.setStyle(Qt::NoPen);
}

void AbstractItem::setPenWidth(int width)
{
	mPen.setWidth(width);
}

void AbstractItem::setPenColor(const QString& text)
{
	mPen.setColor(QColor(text));
}

void AbstractItem::setBrushStyle(const QString& text)
{
	if (text == "Solid")
		mBrush.setStyle(Qt::SolidPattern);
	else if (text == "None")
		mBrush.setStyle(Qt::NoBrush);
}

void AbstractItem::setBrushColor(const QString& text)
{
	mBrush.setColor(QColor(text));
}

void AbstractItem::setPenBrush(const QString& penStyle, int width, const QString& penColor, const QString& brushStyle, const QString& brushColor)
{
	setPenStyle(penStyle);
	setPenWidth(width);
	setPenColor(penColor);
	setBrushStyle(brushStyle);
	setBrushColor(brushColor);
}

void AbstractItem::setXandY(QDomElement& dom, QRectF const &rect)
{
	dom.setAttribute("y1", rect.top());
	dom.setAttribute("x1", rect.left());
	dom.setAttribute("y2", rect.bottom());
	dom.setAttribute("x2", rect.right());
}

QDomElement AbstractItem::setPenBrushToDoc(QDomDocument &document, QString const &domName)
{
	QDomElement dom = document.createElement(domName);
	dom.setAttribute("fill", mBrush.color().name());

	if (mBrush.style() == Qt::NoBrush)
		dom.setAttribute("fill-style", "none");
	if (mBrush.style() == Qt::SolidPattern)
		dom.setAttribute("fill-style", "solid");

	dom.setAttribute("stroke", mPen.color().name());

	dom.setAttribute("stroke-width", mPen.width());

	QString penStyle;
	switch (mPen.style()) {
	case Qt::SolidLine:
		penStyle = "solid";
		break;
	case Qt::DotLine:
		penStyle = "dot";
		break;
	case Qt::DashLine:
		penStyle = "dash";
		break;
	case Qt::DashDotLine:
		penStyle =  "dashdot";
		break;
	case Qt::DashDotDotLine:
		penStyle = "dashdotdot";
		break;
	case Qt::NoPen:
		penStyle = "none";
		break;
	default:
		break;
	}
	dom.setAttribute("stroke-style", penStyle);

	return dom;
}

QRectF AbstractItem::sceneBoundingRectCoord(QPoint const &topLeftPicture)
{
	QRectF itemBoundingRect = calcNecessaryBoundingRect();
	qreal const x1 = scenePos().x() + itemBoundingRect.x() - topLeftPicture.x();
	qreal const y1 = scenePos().y() + itemBoundingRect.y() - topLeftPicture.y();
	return QRectF(x1, y1, itemBoundingRect.width(), itemBoundingRect.height());
}

void AbstractItem::readPenBrush(QDomElement const &docItem)
{
	QString brushStyle = docItem.attribute("fill-style", "");
	if (brushStyle == "solid")
		mBrush.setStyle(Qt::SolidPattern);
	else if (brushStyle == "none")
		mBrush.setStyle(Qt::NoBrush);

	mBrush.setColor(QColor(docItem.attribute("fill", "")));
	mPen.setColor(QColor(docItem.attribute("stroke", "")));
	mPen.setWidth((docItem.attribute("stroke-width", "")).toDouble());

	QString penStyle = docItem.attribute("stroke-style", "");
	if (penStyle == "solid")
		mPen.setStyle(Qt::SolidLine);
	else if (penStyle == "dot")
		mPen.setStyle(Qt::DotLine);
	else if (penStyle == "dash")
		mPen.setStyle(Qt::DashLine);
	else if (penStyle == "dashdot")
		mPen.setStyle(Qt::DashDotLine);
	else if (penStyle == "dashdotdot")
		mPen.setStyle(Qt::DashDotDotLine);
	else if (penStyle == "none")
		mPen.setStyle(Qt::NoPen);
}

QStringList AbstractItem::getPenStyleList()
{
	QStringList penStyleList;
	penStyleList.push_back("Solid");
	penStyleList.push_back("Dot");
	penStyleList.push_back("Dash");
	penStyleList.push_back("DashDot");
	penStyleList.push_back("DashDotDot");
	penStyleList.push_back("None");
	return penStyleList;
}

QStringList AbstractItem::getBrushStyleList()
{
	QStringList brushStyleList;
	brushStyleList.push_back("None");
	brushStyleList.push_back("Solid");
	return brushStyleList;
}
