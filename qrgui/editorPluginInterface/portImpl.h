#pragma once

#include <QtCore/QLineF>
#include <QtCore/QSharedPointer>

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
};
