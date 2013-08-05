#pragma once

#include <QtCore/QSharedPointer>
#include <QtGui/QPainter>

#include "portImpl.h"

/** @brief point port description */
class StatPoint
{
public:
	StatPoint(QPointF const &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl);

	operator QPointF() const;
	void operator= (QPointF const &p);

	void paint(QPainter *painter, QRectF const &contents) const;
	QPointF transformForContents(QRectF const &contents) const;
	QString type() const;

private:
	QPointF mPoint;

	bool mPropX;
	bool mPropY;

	int mInitWidth;
	int mInitHeight;

	QSharedPointer<PortImpl> mImpl;
};

