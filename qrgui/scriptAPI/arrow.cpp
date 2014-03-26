#include "arrow.h"

#include <QtCore>
#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

using namespace qReal;
using namespace gui;

Arrow::Arrow(QPoint const &sourcePoint, QPoint const &destPoint, MainWindow *mainWindow)
	: mArrowSize(10)
	, mSourcePoint(QPoint(sourcePoint))
	, mDestPoint(QPoint(destPoint))
{
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	activateWindow();
	this->setParent(mainWindow);

	update();
}
void Arrow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	QLineF line(mSourcePoint, mDestPoint);

	// Draw the line itself
	painter.setPen(QPen(Qt::black, mArrowSize/2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(line);

	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF mDestArrowP1 = mDestPoint + QPoint(sin(angle - Pi / 3) * mArrowSize,
											  cos(angle - Pi / 3) * mArrowSize);
	QPointF mDestArrowP2 = mDestPoint + QPoint(sin(angle - Pi + Pi / 3) * mArrowSize,
											  cos(angle - Pi + Pi / 3) * mArrowSize);

	painter.setBrush(Qt::black);
	painter.drawPolygon(QPolygonF() << line.p2() << mDestArrowP1 << mDestArrowP2);
}
