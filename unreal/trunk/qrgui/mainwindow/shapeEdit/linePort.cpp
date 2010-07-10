#include "linePort.h"

LinePort::LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent)
	:Line(x1, y1, x2, y2, parent)
{
	mPen.setColor(Qt::blue);
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}
