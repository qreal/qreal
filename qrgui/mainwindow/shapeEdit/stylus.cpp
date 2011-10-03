#include "stylus.h"

Stylus::Stylus(qreal x1, qreal y1, Item* parent):Item(parent), mStylusImpl()
{
	mNeedScalingRect = false;
	mPen.setColor(Qt::black);
	mX1 = x1;
	mY1 = y1;
	mTmpX1 = x1;
	mTmpY1 = y1;
	mDomElementType = pictureType;
}

Stylus::Stylus(Stylus const &other)
	:Item()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mTmpX1 = other.mTmpX1;
	mTmpY1 = other.mTmpY1;
	mListScalePoint = other.mListScalePoint;
	foreach (AbstractItem *line, other.mAbstractListLine) {
		Line *newLine = new Line(*dynamic_cast<Line *>(line));
		mAbstractListLine.append(newLine);
	}
	setPos(other.x(), other.y());
}

Item* Stylus::clone()
{
	Stylus* item = new Stylus(*this);
	return item;
}

void Stylus::addLine(qreal x2, qreal y2)
{
	mX2 = x2;
	mY2 = y2;
	Line *line = new Line(mTmpX1, mTmpY1, mX2, mY2, NULL);
	line->setPen(mPen);
	line->setBrush(mBrush);
	mAbstractListLine.push_back(dynamic_cast<AbstractItem *>(line));
	mTmpX1 = mX2;
	mTmpY1 = mY2;
}

void Stylus::addLineInList(Line *line)
{
	mListLine.push_back(line);
	mAbstractListLine.push_back(dynamic_cast<AbstractItem *>(line));
}

QPainterPath Stylus::shape() const
{
	return mStylusImpl.shape(mAbstractListLine);
}

QRectF Stylus::boundingRect() const
{
	return mStylusImpl.boundingRect(mAbstractListLine);
}

void Stylus::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	mStylusImpl.drawItem(mAbstractListLine, painter, option, widget);
}

void Stylus::drawExtractionForItem(QPainter* painter)
{
	mStylusImpl.drawExtractionForItem(mAbstractListLine, painter);

	/*setPenBrushDriftRect(painter);
	painter->drawPath(shape());
	drawFieldForResizeItem(painter);*/
}

void Stylus::drawFieldForResizeItem(QPainter* painter)
{
	mStylusImpl.drawFieldForResizeItem(painter);
}

void Stylus::drawScalingRects(QPainter* painter)
{
	mStylusImpl.drawScalingRects(painter);
}

void Stylus::setPenStyle(const QString& text)
{
	Item::setPenStyle(text);
	mStylusImpl.setPenStyle(mAbstractListLine, text);
}

void Stylus::setPenWidth(int width)
{
	Item::setPenWidth(width);
	mStylusImpl.setPenWidth(mAbstractListLine, width);
}

void Stylus::setPenColor(const QString& text)
{
	Item::setPenColor(text);
	mStylusImpl.setPenColor(mAbstractListLine, text);
}

void Stylus::setBrushStyle(const QString& text)
{
	Item::setBrushStyle(text);
	mStylusImpl.setBrushStyle(mAbstractListLine, text);
}

void Stylus::setBrushColor(const QString& text)
{
	Item::setBrushColor(text);
	mStylusImpl.setBrushColor(mAbstractListLine, text);
}

QPair<QDomElement, Item::DomElementTypes> Stylus::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement stylus = document.createElement("stylus");
	foreach (AbstractItem *aItem, mAbstractListLine) {
		Line *line = dynamic_cast<Line *>(aItem);
		QDomElement item = (line->generateItem(document, topLeftPicture - QPoint(static_cast<int>(scenePos().x()), static_cast<int>(scenePos().y())))).first;
		stylus.appendChild(item);
	}
	return QPair<QDomElement, Item::DomElementTypes>(stylus, mDomElementType);
}
