#include "arrow.h"

#include <QtCore/QTimer>
#include <QtCore/QtMath>
#include <QtGui/QPainter>

#include <qrutils/mathUtils/math.h>
#include <qrutils/graphicsUtils/animatedEffects.h>

const qreal arrowSize = 10;

using namespace qReal;
using namespace gui;
using namespace mathUtils;
using namespace graphicsUtils;

Arrow::Arrow(const QPoint &sourcePoint, const QPoint &destPoint, int lifeTime, QWidget *parent)
	: QWidget(parent)
	, mSourcePoint(sourcePoint)
	, mDestPoint(destPoint)
	, mDuration(lifeTime / 2)  // lifeTime = showing + fading out
{
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	activateWindow();

	QTimer::singleShot(mDuration, this, SLOT(disappear()));
	QTimer::singleShot(lifeTime, this, SLOT(deleteLater()));
}

void Arrow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	QLineF line(mSourcePoint, mDestPoint);

	// Draw the line itself
	painter.setPen(QPen(Qt::black, arrowSize / 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(line);

	qreal angle = qAcos(line.dx() / line.length());
	if (line.dy() >= 0) {
		angle = 2 * pi - angle;
	}

	const QPointF destArrowFirst = mDestPoint + QPoint(qSin(angle - pi / 3) * arrowSize
			, qCos(angle - pi / 3) * arrowSize);
	const QPointF destArrowSecond = mDestPoint + QPoint(qSin(angle - 2 * pi / 3) * arrowSize
			, qCos(angle - 2 * pi / 3) * arrowSize);

	painter.setBrush(Qt::black);
	painter.drawPolygon(QPolygonF() << line.p2() << destArrowFirst << destArrowSecond);
}

void Arrow::disappear()
{
	AnimatedEffects::disappear(this, mDuration);
}
