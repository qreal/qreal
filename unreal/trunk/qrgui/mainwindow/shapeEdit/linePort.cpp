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
	QRectF rect = sceneBoundingRectCoord(topLeftPicture);

	QDomElement start  = document.createElement("start");
	linePort.appendChild(start);
	start.setAttribute("starty", rect.top());
	start.setAttribute("startx", rect.left());

	QDomElement end  = document.createElement("end");
	linePort.appendChild(end);
	end.setAttribute("endy", rect.bottom());
	end.setAttribute("endx", rect.right());

	return QPair<QDomElement, Item::DomElementTypes>(linePort, mDomElementType);
}
