#include "item.h"
#include "math.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>

Item::Item(QGraphicsItem* parent) : QGraphicsItem(parent), mDomElementType(noneType), mDragState(None)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	mBrush.setColor(mPen.color());
}

int Item::sign(int x)
{
	return x > 0 ? 1 : -1;
}

qreal Item::length(QPointF const &point1, QPointF const &point2)
{
	return sqrt(pow((point1.x() - point2.x()), 2) + pow((point1.y() - point2.y()), 2));
}

void Item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	painter->setPen(mPen);
	painter->setBrush(mBrush);
	drawItem(painter, option, widget);
	if (option->state & QStyle::State_Selected) {
		painter->save();
		QPen pen(Qt::red);
		pen.setWidth(3);
		painter->setPen(pen);
		drawExtractionForItem(painter);
		painter->restore();
	}
}

void Item::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(mX1, mY1);
	painter->drawPoint(mX1, mY2);
	painter->drawPoint(mX2, mY1);
	painter->drawPoint(mX2, mY2);
	drawFieldForResizeItem(painter);
}

void Item::drawFieldForResizeItem(QPainter* painter)
{
	setPenBrushDriftRect(painter);
	qreal x1= boundingRect().left();
	qreal x2 = boundingRect().right();
	qreal y1 = boundingRect().top();
	qreal y2 = boundingRect().bottom();
	painter->drawRect(x1, y1, resizeDrift, resizeDrift);
	painter->drawRect(x2 - resizeDrift, y2 - resizeDrift, resizeDrift, resizeDrift);
	painter->drawRect(x1, y2 - resizeDrift, resizeDrift, resizeDrift);
	painter->drawRect(x2 - resizeDrift, y1, resizeDrift, resizeDrift);
}

void Item::setPenBrushDriftRect(QPainter* painter)
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

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseMoveEvent(event);
}

QPen Item::pen() const
{
	return mPen;
}

QBrush Item::brush() const
{
	return mBrush;
}

void Item::setBrush(QBrush const &brush)
{
	mBrush = brush;
}

void Item::setPen(QPen const &pen)
{
	mPen = pen;
}

void Item::swap(qreal &x, qreal &y)
{
	qreal tmp = x;
	x = y;
	y = tmp;
}

void Item::setX1andY1(qreal x, qreal y)
{
	mX1 = x;
	mY1 = y;
	update();
}

void Item::setX1andY2(qreal x, qreal y)
{
	mX1 = x;
	mY2 = y;
	update();
}

void Item::setX2andY1(qreal x, qreal y)
{
	mX2 = x;
	mY1 = y;
	update();
}

void Item::setX2andY2(qreal x, qreal y)
{
	mX2 = x;
	mY2 = y;
	update();
}

void Item::setNoneDragState()
{
	mDragState = None;
}

void Item::reshapeRectWithShift()
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

void Item::changeDragState(qreal x, qreal y)
{
	if (QRectF(QPointF(mX1 + scenePos().x(), mY1 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = TopLeft;
	else if (QRectF(QPointF(mX2 + scenePos().x(), mY1 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = TopRight;
	else if (QRectF(QPointF(mX1 + scenePos().x(), mY2 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = BottomLeft;
	else if (QRectF(QPointF(mX2 + scenePos().x(), mY2 + scenePos().y()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
		mDragState = BottomRight;
	else
		mDragState = None;
}

Item::DragState Item::getDragState() const
{
	return mDragState;
}

void Item::calcResizeItem(QGraphicsSceneMouseEvent *event)
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

void Item::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState != None)
		calcResizeItem(event);
	else
		setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Item::setXandY(QDomElement& dom, QRectF const &rect)
{
	dom.setAttribute("y1", rect.top());
	dom.setAttribute("x1", rect.left());
	dom.setAttribute("y2", rect.bottom());
	dom.setAttribute("x2", rect.right());
}

QDomElement Item::setPenBrushToDoc(QDomDocument &document, QString const &domName)
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

QRectF Item::sceneBoundingRectCoord(QPointF const &topLeftPicture)
{
	qreal const x1 = scenePos().x() + boundingRect().x() - topLeftPicture.x();
	qreal const y1 = scenePos().y() + boundingRect().y() - topLeftPicture.y();
	return QRectF(x1, y1, boundingRect().width(), boundingRect().height());
}

void Item::readPenBrush(QDomElement const &docItem)
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

QStringList Item::getPenStyleList()
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

QStringList Item::getBrushStyleList()
{
	QStringList brushStyleList;
	brushStyleList.push_back("None");
	brushStyleList.push_back("Solid");
	return brushStyleList;
}

void Item::setPenStyle(const QString& text)
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

void Item::setPenWidth(int width)
{
	mPen.setWidth(width);
}

void Item::setPenColor(const QString& text)
{
	mPen.setColor(QColor(text));
}

void Item::setBrushStyle(const QString& text)
{
	if (text == "Solid")
		mBrush.setStyle(Qt::SolidPattern);
	else if (text == "None")
		mBrush.setStyle(Qt::NoBrush);
}

void Item::setBrushColor(const QString& text)
{
	mBrush.setColor(QColor(text));
}

void Item::setPenBrush(const QString& penStyle, int width, const QString& penColor, const QString& brushStyle, const QString& brushColor)
{
	setPenStyle(penStyle);
	setPenWidth(width);
	setPenColor(penColor);
	setBrushStyle(brushStyle);
	setBrushColor(brushColor);
}
