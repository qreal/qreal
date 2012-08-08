#include "borderChecker.h"
#include "nodeElement.h"

BorderChecker::BorderChecker(const NodeElement* const node)
	: mNode(node), mBorderValues(node->borderValues())
	, mXHor(mBorderValues[0])
	, mYHor(mBorderValues[1])
	, mXVert(mBorderValues[2])
	, mYVert(mBorderValues[3])
{
}

bool BorderChecker::checkLowerBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x) && (currentY >= rc.y() + rc.height() - y)
			&& (currentY <= rc.y() + rc.height() + y);
}

bool BorderChecker::checkUpperBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x) && (currentY >= rc.y() - y)
			&& (currentY <= rc.y() + y);
}

bool BorderChecker::checkLeftBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() - x) && (currentX <= rc.x() + x) && (currentY >= rc.y() + y)
			&& (currentY <= rc.y() + rc.height() - y);
}

bool BorderChecker::checkRightBorder(QPointF const &point, double const x, double const y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + rc.width() - x) && (currentX <= rc.x() + rc.width() + x) && (currentY >= rc.y() + y)
			&& (currentY <= rc.y() + rc.height() - y);
}

bool BorderChecker::checkNoBorderX(QPointF const &point, double const y) const
{
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentY >= rc.y() + y) && (currentY <= rc.y() + rc.height() - y);
}

bool BorderChecker::checkNoBorderY(QPointF const &point, double const x) const
{
	double currentX = point.x();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x);
}
