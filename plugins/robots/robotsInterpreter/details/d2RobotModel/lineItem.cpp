#include "lineItem.h"
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

LineItem::LineItem(QPointF begin, QPointF end)
	: mLineImpl()
{
	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
}

void LineItem::setPrivateData()
{
	mPen.setColor(Qt::green);
	mPen.setStyle(Qt::SolidLine);
	mSerializeName = "line";
}

QRectF LineItem::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

void LineItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mLineImpl.drawItem(painter, mX1, mY1, mX2, mY2);
}

void LineItem::drawExtractionForItem(QPainter* painter)
{
	mLineImpl.drawPointExtractionForItem(painter, mX1, mY1, mX2, mY2);
	setPenBrushDriftRect(painter);
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, mX1, mY1, mX2, mY2);

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
}

QPainterPath LineItem::shape() const
{
	return mLineImpl.shape(drift, mX1, mY1, mX2, mY2);
}

void LineItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == TopLeft || mDragState == BottomRight)
		AbstractItem::resizeItem(event);
}

void LineItem::reshapeRectWithShift()
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
		AbstractItem::reshapeRectWithShift();
}

QDomElement LineItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
        QDomElement lineNode = setPenBrushToDoc(document, mSerializeName);
        lineNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x()) + ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
        lineNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x()) + ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
        return lineNode;
}

void LineItem::deserialize(QDomElement const &element)
{
	QString const beginStr = element.attribute("begin", "0:0");
	QStringList splittedStr = beginStr.split(":");
	int x = splittedStr[0].toInt();
	int y = splittedStr[1].toInt();
	QPointF const begin = QPointF(x, y);

	QString const endStr = element.attribute("end", "0:0");
	splittedStr = endStr.split(":");
	x = splittedStr[0].toInt();
	y = splittedStr[1].toInt();
	QPointF const end = QPointF(x, y);

	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();

	readPenBrush(element);
}

void LineItem::setSerializeName(QString name)
{
    mSerializeName = name;
}
