#include "linePort.h"

LinePort::LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent)
	:Line(x1, y1, x2, y2, parent)
{
	mPen.setColor(Qt::blue);
	mDomElementType = portType;
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

QPair<QDomElement, Item::DomElementTypes> LinePort::generateItem(QDomDocument &document, QPointF const &topLeftPicture)
{
	QDomElement linePort = document.createElement("linePort");

	qreal const x1 = scenePos().x() + line().x1() - topLeftPicture.x();
	qreal const y1 = scenePos().y() + line().y1() - topLeftPicture.y();
	qreal const x2 = scenePos().x() + line().x2() - topLeftPicture.x();
	qreal const y2 = scenePos().y() + line().y2() - topLeftPicture.y();

	QDomElement start  = document.createElement("start");
	linePort.appendChild(start);
	start.setAttribute("starty", y1);
	start.setAttribute("startx", x1);

	QDomElement end  = document.createElement("end");
	linePort.appendChild(end);
	end.setAttribute("endy", y2);
	end.setAttribute("endx", x2);

	return QPair<QDomElement, Item::DomElementTypes>(linePort, mDomElementType);
}
