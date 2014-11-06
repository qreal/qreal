#pragma once

#include "plugins/editorPluginInterface/portHelpers.h"

/** @brief point port description */
class StatPoint : public PortInterface
{
public:
	StatPoint(QPointF const &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl);

	operator QPointF() const;
	void operator= (QPointF const &p);

	virtual void paint(QPainter *painter, QRectF const &contents) const;
	QPointF transformForContents(QRectF const &contents) const;

private:
	QPointF mPoint;

	bool mPropX;
	bool mPropY;

	int mInitWidth;
	int mInitHeight;
};

