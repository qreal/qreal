#include <QtCore/QVector>
#include <QtCore/QPointF>
#include <QtGui/QPainter>

#include "spacer.h"

using namespace qReal::widgetsEdit;

Spacer::Spacer(Qt::Orientation orientation, ToolController *controller)
	: Tool(new QWidget(), controller)
	, mOrientation(orientation)
{
	mSpacer = dynamic_cast<QWidget *>(widget());
	mSpacer->setGeometry(0, 0
		, widthFromOrientation(orientation)
		, heightFromOrientation(orientation));
	mSpacer->setSizePolicy(hPolicyFromOrientation(orientation)
		, vPolicyFromOrientation(orientation));
	mSpacer->setMinimumHeight(10.0);
	mSpacer->setMaximumWidth(10.0);
	mIcon = QIcon(iconPathFromOrientation(mOrientation));
	mTitle = titleFromOrientation(mOrientation);
}

void Spacer::paint(QPainter *painter
		, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Tool::paint(painter, option, widget);
	painter->save();
	painter->setPen(Qt::blue);
	qreal const width = geometry().width();
	qreal const height = geometry().height();
	if (Qt::Horizontal == mOrientation) {
		qreal halfHeight = height / 2;
		qreal startWidth = 3.0;
		qreal finishWidth = width - 3.0;
		painter->drawLine(startWidth, halfHeight - 5.0, startWidth, halfHeight + 5.0);
		painter->drawLine(finishWidth, halfHeight - 5.0, finishWidth, halfHeight + 5.0);
		qreal x = startWidth;
		qreal step = 2.0;
		QVector<QPointF> springPoints;
		int sign = 1;
		while (x < finishWidth) {
			springPoints << QPointF(x, halfHeight + sign * step / 2);
			x += step;
			sign *= -1;
		}
		springPoints << QPointF(finishWidth, halfHeight + sign * step / 2);
		painter->drawPolyline(springPoints);
	} else {
		qreal halfWidth = width / 2;
		qreal startHeight = 3.0;
		qreal finishHeight = height - 3.0;
		painter->drawLine(halfWidth - 5.0, startHeight, halfWidth + 5.0, startHeight);
		painter->drawLine(halfWidth - 5.0, finishHeight, halfWidth + 5.0, finishHeight);
		qreal y = startHeight;
		qreal step = 4.0;
		QVector<QPointF> springPoints;
		int sign = 1;
		while (y < finishHeight) {
			springPoints << QPointF(halfWidth + sign * step / 2, y);
			y += step;
			sign *= -1;
		}
		springPoints << QPointF(halfWidth + sign * step / 2, finishHeight);
		painter->drawPolyline(springPoints);
	}
	painter->restore();
}

QString Spacer::titleFromOrientation(Qt::Orientation orientation)
{
	return (Qt::Horizontal == orientation)
			? tr("Horizontal Spacer")
			: tr("Vertical Spacer");
}

QString Spacer::iconPathFromOrientation(Qt::Orientation orientation)
{
	return (Qt::Horizontal == orientation)
			? tr(":/icons/widgetsEditor/horizontalSpacer.png")
			: tr(":/icons/widgetsEditor/verticalSpacer.png");
}

int Spacer::widthFromOrientation(Qt::Orientation orientation)
{
	return (Qt::Horizontal == orientation)
			? SPACER_DEFAULT_WIDTH_ACTIVE
			: SPACER_DEFAULT_WIDTH_INACTIVE;
}

int Spacer::heightFromOrientation(Qt::Orientation orientation)
{
	return (Qt::Horizontal == orientation)
			? SPACER_DEFAULT_HEIGHT_INACTIVE
			: SPACER_DEFAULT_HEIGHT_ACTIVE;
}

QSizePolicy::Policy Spacer::hPolicyFromOrientation(Qt::Orientation orientation)
{
	return (Qt::Horizontal == orientation)
			? QSizePolicy::Expanding
			: QSizePolicy::Minimum;
}

QSizePolicy::Policy Spacer::vPolicyFromOrientation(Qt::Orientation orientation)
{
	return (Qt::Horizontal == orientation)
			? QSizePolicy::Minimum
			: QSizePolicy::Expanding;
}
