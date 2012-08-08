#pragma once

#include <QtCore/QPointF>

class NodeElement;

class BorderChecker
{
public:
	BorderChecker(const NodeElement* const node);

	bool checkLowerBorder(QPointF const &point, double const x, double const y) const;
	bool checkUpperBorder(QPointF const &point, double const x, double const y) const;
	bool checkLeftBorder(QPointF const &point, double const x, double const y) const;
	bool checkRightBorder(QPointF const &point, double const x, double const y) const;
	bool checkNoBorderX(QPointF const &point, double const y) const; // TODO: rename
	bool checkNoBorderY(QPointF const &point, double const x) const;

private:
	const NodeElement* const mNode;
	QList<double const> const mBorderValues;

	double const mXHor;
	double const mYHor;
	double const mXVert;
	double const mYVert;
};
