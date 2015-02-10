#pragma once

#include "plugins/editorPluginInterface/portHelpers.h"

/** @brief line port description */
class StatLine : public PortInterface
{
public:
	StatLine(const QLineF &line, bool propX1, bool propY1, bool propX2, bool propY2, int initWidth, int initHeight
			, PortImpl *impl);

	operator QLineF () const;
	void operator= (const QLineF &l);

	virtual void paint(QPainter *painter, const QRectF &contents) const;
	QLineF transformForContents(const QRectF &contents) const;

private:
	QLineF mLine;

	bool mPropX1;
	bool mPropY1;
	bool mPropX2;
	bool mPropY2;

	int mInitWidth;
	int mInitHeight;
};
