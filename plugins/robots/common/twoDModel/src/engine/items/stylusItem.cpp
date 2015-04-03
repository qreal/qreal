#include "stylusItem.h"

using namespace twoDModel::items;
using namespace graphicsUtils;

StylusItem::StylusItem(qreal x1, qreal y1)
	: mStylusImpl()
{
	QPen pen(this->pen());
	pen.setColor(Qt::black);
	pen.setCapStyle(Qt::RoundCap);
	setPen(pen);
	setX1(x1);
	setY1(y1);
	mTmpX1 = x1;
	mTmpY1 = y1;
}

AbstractItem *StylusItem::clone() const
{
	const auto cloned = new StylusItem(x1(), y1());
	cloned->mTmpX1 = mTmpX1;
	cloned->mTmpY1 = mTmpY1;
	cloned->mBoundingRect = mBoundingRect;
	for (const auto item : mAbstractListLine) {
		cloned->mAbstractListLine.push_back(item);
	}

	return cloned;
}

void StylusItem::addLine(qreal x2, qreal y2)
{
	setX2(x2);
	setY2(y2);
	LineItem *line = new LineItem(QPointF(mTmpX1, mTmpY1), QPointF(this->x2(), this->y2()));
	line->setPen(pen());
	line->setBrush(brush());
	line->setSerializeName(QString("stylusLine"));
	mAbstractListLine.push_back(line);
	recalculateProperties();
	mTmpX1 = this->x2();
	mTmpY1 = this->y2();
}

QPainterPath StylusItem::shape() const
{
	return mStylusImpl.shape(mAbstractListLine);
}

QRectF StylusItem::boundingRect() const
{
	return mBoundingRect;
}

void StylusItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	mStylusImpl.drawItem(mAbstractListLine, painter, option, widget);
}

void StylusItem::drawExtractionForItem(QPainter* painter)
{
	mStylusImpl.drawExtractionForItem(mAbstractListLine, painter);
}

void StylusItem::drawFieldForResizeItem(QPainter* painter)
{
	mStylusImpl.drawFieldForResizeItem(painter);
}

void StylusItem::drawScalingRects(QPainter* painter)
{
	mStylusImpl.drawScalingRects(painter);
}

void StylusItem::setPenStyle(const QString& text)
{
	AbstractItem::setPenStyle(text);
	mStylusImpl.setPenStyle(mAbstractListLine, text);
}

void StylusItem::setPenWidth(int width)
{
	AbstractItem::setPenWidth(width);
	mStylusImpl.setPenWidth(mAbstractListLine, width);
}

void StylusItem::setPenColor(const QString& text)
{
	AbstractItem::setPenColor(text);
	mStylusImpl.setPenColor(mAbstractListLine, text);
}

void StylusItem::setBrushStyle(const QString& text)
{
	AbstractItem::setBrushStyle(text);
	mStylusImpl.setBrushStyle(mAbstractListLine, text);
}

void StylusItem::setBrushColor(const QString& text)
{
	AbstractItem::setBrushColor(text);
	mStylusImpl.setBrushColor(mAbstractListLine, text);
}

QDomElement StylusItem::serialize(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement stylusNode = setPenBrushToDoc(document, "stylus");
	AbstractItem::serialize(stylusNode);
	for (AbstractItem *abstractItem : mAbstractListLine) {
			LineItem *line = dynamic_cast<LineItem *>(abstractItem);
			line->setSerializeName("stylusLine");
			QDomElement item = line->serialize(document, topLeftPicture - QPoint(static_cast<int>(scenePos().x()), static_cast<int>(scenePos().y())));
			stylusNode.appendChild(item);
	}

	return stylusNode;
}

void StylusItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	mAbstractListLine.clear();
	recalculateProperties();

	readPenBrush(element);
	QPen pen(this->pen());
	pen.setCapStyle(Qt::RoundCap);
	setPen(pen);
	QDomNodeList stylusAttributes = element.childNodes();
	for (int i = 0; i < stylusAttributes.length(); ++i) {
			QDomElement type = stylusAttributes.at(i).toElement();
			if (type.tagName() == "stylusLine") {
				LineItem * const line = new LineItem(QPointF(0, 0), QPointF(0, 0));
				line->deserialize(type);
				line->setPen(this->pen());
				mAbstractListLine.append(line);
				recalculateProperties();
			} else {
//				Tracer::debug(tracer::enums::d2Model, "StylusItem::deserialize", "Incorrect stylus tag");
			}
	}
}

void StylusItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	mStylusImpl.resizeItem(event);
}

void StylusItem::recalculateProperties()
{
	mBoundingRect = mStylusImpl.boundingRect(mAbstractListLine);
}
