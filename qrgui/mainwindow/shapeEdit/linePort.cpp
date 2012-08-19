#include "linePort.h"

LinePort::LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent)
	:Line(x1, y1, x2, y2, parent)
{
	mNeedScalingRect = true;
	mPen.setColor(Qt::blue);
	mDomElementType = portType;
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

LinePort::LinePort(LinePort const &other)
	:Line(other)
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = portType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* LinePort::clone()
{
	LinePort* item = new LinePort(*this);
	return item;
}

QPair<QDomElement, Item::DomElementTypes> LinePort::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement linePort = document.createElement("linePort");

	qreal const x1 = scenePos().x() + line().x1() - topLeftPicture.x();
	qreal const y1 = scenePos().y() + line().y1() - topLeftPicture.y();
	qreal const x2 = scenePos().x() + line().x2() - topLeftPicture.x();
	qreal const y2 = scenePos().y() + line().y2() - topLeftPicture.y();

	QPair<QPair<QString, QString>, QPair<QString, QString> > res = setXandYBefore(QRectF(x1, y1, x2 - x1, y2 - y1).normalized().toRect());
	QDomElement start  = document.createElement("start");
	linePort.appendChild(start);
	start.setAttribute("starty", res.first.second);
	start.setAttribute("startx", res.first.first);

	QDomElement end  = document.createElement("end");
	linePort.appendChild(end);
	end.setAttribute("endy", res.second.second);
	end.setAttribute("endx", res.second.first);

	return QPair<QDomElement, Item::DomElementTypes>(linePort, mDomElementType);
}
