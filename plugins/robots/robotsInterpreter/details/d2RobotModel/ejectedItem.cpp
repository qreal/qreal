#include "ejectedItem.h"
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

EjectedItem::EjectedItem(QPointF begin, QPointF end)
	: mRectImpl()
{
	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
}

void EjectedItem::setPrivateData()
{
	mPen.setColor(Qt::green);
	mPen.setStyle(Qt::SolidLine);
	mBrush.setColor(Qt::blue);
	mBrush.setStyle(Qt::SolidPattern);
	mSerializeName = "ejectedItem";
}

QRectF EjectedItem::calcNecessaryBoundingRect() const
{
	return QRectF(qMin(mX1, mX2), qMin(mY1, mY2), abs(mX2 - mX1), abs(mY2 - mY1));
}

QRectF EjectedItem::boundingRect() const
{
	return mRectImpl.boundingRect(mX1, mY1, mX2, mY2, drift);
}

void EjectedItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mRectImpl.drawRectItem(painter, mX1, mY1, mX2, mY2);
}

QDomElement EjectedItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
		QDomElement rectNode = setPenBrushToDoc(document, mSerializeName);
		rectNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x()) + ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
		rectNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x()) + ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
		return rectNode;
}

void EjectedItem::deserialize(QDomElement const &element)
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

void EjectedItem::robotChangedPosition(QPointF const &newPos)
{
	qDebug() << "qwerty" << newPos;
}
