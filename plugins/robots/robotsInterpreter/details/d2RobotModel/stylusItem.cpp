#include "stylusItem.h"
#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

StylusItem::StylusItem(qreal x1, qreal y1)
	: mStylusImpl()
{
	mPen.setColor(Qt::black);
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
	mAbstractListLine.push_back(line);
	mTmpX1 = mX2;
	mTmpY1 = mY2;
}

QPainterPath StylusItem::shape() const
{
	return mStylusImpl.shape(mAbstractListLine);
}

QRectF StylusItem::boundingRect() const
{
	return mStylusImpl.boundingRect(mAbstractListLine);
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

QDomElement StylusItem::serialize(QDomDocument &document) /*const*/
{
	QDomElement stylusNode = setPenBrushToDoc(document, "stylus");
	foreach (AbstractItem *abstractItem, mAbstractListLine) {
		LineItem *line = dynamic_cast<LineItem *>(abstractItem);
		QDomElement item = line->serialize(document);
		stylusNode.appendChild(item);
	}
	return stylusNode;
}

void StylusItem::deserialize(QDomElement const &element)
{
    mAbstractListLine.clear();

    readPenBrush(element);
    QDomNodeList stylusAttributes = element.childNodes();
    for (unsigned i = 0; i < stylusAttributes.length(); ++i) {
            QDomElement type = stylusAttributes.at(i).toElement();
            if (type.tagName() == "line") {
                    LineItem* line = new LineItem(QPointF(0, 0), QPointF(0, 0));
                    line->deserialize(type);
                    mAbstractListLine.append(line);
            }
            else
                    qDebug() << "Incorrect stylus tag";
    }
}

void StylusItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	mStylusImpl.resizeItem(event);
}
