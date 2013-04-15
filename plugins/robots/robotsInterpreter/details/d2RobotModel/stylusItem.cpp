#include "stylusItem.h"
#include "../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace d2Model;
using namespace graphicsUtils;

StylusItem::StylusItem(qreal x1, qreal y1)
	: mStylusImpl()
{
	mPen.setColor(Qt::black);
	mPen.setCapStyle(Qt::RoundCap);
	mX1 = x1;
	mY1 = y1;
	mTmpX1 = x1;
	mTmpY1 = y1;
}

void StylusItem::addLine(qreal x2, qreal y2)
{
	mX2 = x2;
	mY2 = y2;
	LineItem *line = new LineItem(QPointF(mTmpX1, mTmpY1), QPointF(mX2, mY2));
	line->setPen(mPen);
	line->setBrush(mBrush);
	line->setSerializeName(QString("stylusLine"));
	mAbstractListLine.push_back(line);
	recalculateProperties();
	mTmpX1 = mX2;
	mTmpY1 = mY2;
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

QDomElement StylusItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
		QDomElement stylusNode = setPenBrushToDoc(document, "stylus");
		foreach (AbstractItem *abstractItem, mAbstractListLine) {
				LineItem *line = dynamic_cast<LineItem *>(abstractItem);
				line->setSerializeName("stylusLine");
				QDomElement item = line->serialize(document, topLeftPicture - QPoint(static_cast<int>(scenePos().x()), static_cast<int>(scenePos().y())));
				stylusNode.appendChild(item);
		}
		return stylusNode;
}

void StylusItem::deserialize(QDomElement const &element)
{
	mAbstractListLine.clear();
	recalculateProperties();

	readPenBrush(element);
	mPen.setCapStyle(Qt::RoundCap);
	QDomNodeList stylusAttributes = element.childNodes();
	for (int i = 0; i < stylusAttributes.length(); ++i) {
			QDomElement type = stylusAttributes.at(i).toElement();
			if (type.tagName() == "stylusLine") {
					LineItem* line = new LineItem(QPointF(0, 0), QPointF(0, 0));
					line->deserialize(type);
					line->setPen(mPen);
					mAbstractListLine.append(line);
					recalculateProperties();
			}
			else
				Tracer::debug(tracer::d2Model, "StylusItem::deserialize", "Incorrect stylus tag");
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
