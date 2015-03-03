#pragma once

#include "plugins/editorPluginInterface/portHelpers.h"

/** @brief point port description */
class StatPoint : public PortInterface
{
public:
	StatPoint(const QPointF &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl);

	operator QPointF() const;
	void operator= (const QPointF &p);

	virtual void paint(QPainter *painter, const QRectF &contents) const;
	QPointF transformForContents(const QRectF &contents) const;

private:
	QPointF mPoint;

	bool mPropX;
	bool mPropY;

	int mInitWidth;
	int mInitHeight;
};

