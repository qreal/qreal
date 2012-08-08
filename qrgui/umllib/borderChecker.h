#pragma once

#include <QtCore/QPointF>
#include <QList>

class NodeElement;

class BorderChecker
{
public:
	BorderChecker(const NodeElement* const node);

	bool checkLowerBorder(QPointF const &point) const;
	bool checkUpperBorder(QPointF const &point) const;
	bool checkLeftBorder(QPointF const &point) const;
	bool checkRightBorder(QPointF const &point) const;
	bool checkNoBorderX(QPointF const &point) const; // TODO: rename
	bool checkNoBorderY(QPointF const &point) const;

private:
	const NodeElement* const mNode;
	QList<double> const mBorderValues;

	double const mXHor;
	double const mYHor;
	double const mXVert;
	double const mYVert;
};
