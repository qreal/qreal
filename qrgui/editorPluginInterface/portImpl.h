#pragma once

#include <QtCore/QSharedPointer>
#include <QtGui/QPainter>

class PortImpl
{
public:
	virtual ~PortImpl() {}

	virtual QString type() const = 0;
};

class NonTyped : public PortImpl
{
public:
	virtual QString type() const {
		return "NonTyped";
	}
};

/** @brief point port description */
struct StatPoint
{
	QPointF point;

	/// Denote is the port need to be scaled on X dimension or not.
	/// 'true' - means that the port isn't need to be scaled.
	/// 'false' - means that the port is need to be scaled.
	bool prop_x;

	/// Denote is the port need to be scaled on Y dimension or not.
	/// 'true' - means that the port isn't need to be scaled.
	/// 'false' - means that the port is need to be scaled.
	bool prop_y;

	int initWidth;
	int initHeight;

	QSharedPointer<PortImpl> impl;

	StatPoint() : point(QPointF(0, 0)), prop_x(false), prop_y(false),
		initWidth(1), initHeight(1), impl(NULL) {}

	operator QPointF () const
	{
		return point;
	}

	void operator = (QPointF const &p)
	{
		point = p;
		prop_x = false;
		prop_y = false;
		initHeight = 1;
		initWidth = 1;
	}

	void paint(QPainter *painter, QRectF const &contents) const
	{
		qreal const x = point.x() * (prop_x ? initWidth : contents.width());
		qreal const y = point.y() * (prop_y ? initHeight : contents.height());

		QLineF pointToDraw(x - 0.1, y - 0.1, x + 0.1, y + 0.1);

		painter->save();
		QPen pen = painter->pen();
		pen.setColor(QColor("#c3dcc4"));
		pen.setWidth(12);
		painter->setPen(pen);
		painter->drawLine(pointToDraw);
		pen.setColor(QColor("#465945"));
		pen.setWidth(3);
		painter->setPen(pen);
		painter->drawLine(pointToDraw);
		painter->restore();
	}
};

/** @brief line port description */
struct StatLine
{
	QLineF line;
	bool prop_x1;
	bool prop_y1;
	bool prop_x2;
	bool prop_y2;
	int initWidth;
	int initHeight;

	QSharedPointer<PortImpl> impl;

	StatLine() : line(QLineF(0, 0, 0, 0)), prop_x1(false), prop_y1(false),
		prop_x2(false), prop_y2(false), initWidth(1), initHeight(1), impl(NULL) {}

	operator QLineF () const
	{
		return line;
	}

	void operator = (QLineF const &l)
	{
		line = l;
		prop_x1 = false;
		prop_x2 = false;
		prop_y1 = false;
		prop_y2 = false;
		initHeight = 1;
		initWidth = 1;
	}

	void paint(QPainter *painter, QRectF const &contents) const
	{
		qreal const x1 = line.x1() * (prop_x1 ? initWidth : contents.width());
		qreal const y1 = line.y1() * (prop_y1 ? initHeight : contents.height());

		qreal const x2 = line.x2() * (prop_x2 ? initWidth : contents.width());
		qreal const y2 = line.y2() * (prop_y2 ? initHeight : contents.height());

		QLineF lineToDraw(x1, y1, x2, y2);

		painter->save();
		QPen pen = painter->pen();
		pen.setColor(QColor("#c3dcc4"));
		pen.setWidth(7);
		painter->setPen(pen);
		painter->drawLine(lineToDraw);
		pen.setColor(QColor("#465945"));
		pen.setWidth(1);
		painter->setPen(pen);
		painter->drawLine(lineToDraw);
		painter->restore();
	}
};
