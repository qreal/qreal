#include "ellipseItem.h"

using namespace twoDModel::items;
using namespace graphicsUtils;

int const currentResizeDrift = resizeDrift;
int const currentDrift = drift / 2;

EllipseItem::EllipseItem(QPointF const &begin, QPointF const &end)
	: mEllipseImpl()
{
	mPen.setColor(Qt::black);
	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
}

void EllipseItem::setPrivateData()
{
	mPen.setColor(Qt::blue);
	mPen.setStyle(Qt::SolidLine);
	mBrush.setColor(Qt::white);
	mBrush.setStyle(Qt::NoBrush);
}

QRectF EllipseItem::calcNecessaryBoundingRect() const
{
	return QRectF(qMin(mX1, mX2), qMin(mY1, mY2), abs(mX2 - mX1), abs(mY2 - mY1));
}

QRectF EllipseItem::boundingRect() const
{
	return mEllipseImpl.boundingRect(mX1, mY1, mX2, mY2, currentDrift);
}

void EllipseItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mEllipseImpl.drawEllipseItem(painter, mX1, mY1, mX2, mY2);
}

QDomElement EllipseItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement ellipseNode = setPenBrushToDoc(document, "ellipse");
	ellipseNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x())
			 + ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
	ellipseNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x())
			 + ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
	return ellipseNode;
}

void EllipseItem::deserialize(QDomElement const &element)
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
