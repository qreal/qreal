#include "arrow.h"

#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <qrutils/mathUtils/math.h>

qreal const arrowSize = 10;

using namespace qReal;
using namespace gui;
using namespace mathUtils;

Arrow::Arrow(QPoint const &sourcePoint, QPoint const &destPoint, QWidget *parent)
	: QWidget(parent)
	, mSourcePoint(sourcePoint)
	, mDestPoint(destPoint)
{
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	activateWindow();
}

void Arrow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	QLineF line(mSourcePoint, mDestPoint);

	// Draw the line itself
	painter.setPen(QPen(Qt::black, arrowSize / 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(line);

	qreal angle = acos(line.dx() / line.length());
	if (line.dy() >= 0) {
		angle = 2*pi - angle;
	}

	QPointF const destArrowFirst = mDestPoint + QPoint(sin(angle - pi / 3) * arrowSize,
													cos(angle - pi / 3) * arrowSize);
	QPointF const destArrowSecond = mDestPoint + QPoint(sin(angle - pi + pi / 3) * arrowSize,
													cos(angle - pi + pi / 3) * arrowSize);

	painter.setBrush(Qt::black);
	painter.drawPolygon(QPolygonF() << line.p2() << destArrowFirst << destArrowSecond);
}
