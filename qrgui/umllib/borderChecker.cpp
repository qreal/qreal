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

bool BorderChecker::checkLowerBorder(QPointF const &point) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + mXHor) && (currentX <= rc.x() + rc.width() - mXHor) && (currentY >= rc.y() + rc.height() - mYHor)
			&& (currentY <= rc.y() + rc.height() + mYHor);
}

bool BorderChecker::checkUpperBorder(QPointF const &point) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + mXHor) && (currentX <= rc.x() + rc.width() - mXHor) && (currentY >= rc.y() - mYHor)
			&& (currentY <= rc.y() + mYHor);
}

bool BorderChecker::checkLeftBorder(QPointF const &point) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() - mXVert) && (currentX <= rc.x() + mXVert) && (currentY >= rc.y() + mYVert)
			&& (currentY <= rc.y() + rc.height() - mYVert);
}

bool BorderChecker::checkRightBorder(QPointF const &point) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + rc.width() - mXVert) && (currentX <= rc.x() + rc.width() + mXVert) && (currentY >= rc.y() + mYVert)
			&& (currentY <= rc.y() + rc.height() - mYVert);
}

bool BorderChecker::checkNoBorderX(QPointF const &point) const
{
	double currentY = point.y();
	QRectF rc = mNode->boundingRect();
	return (currentY >= rc.y() + mYVert) && (currentY <= rc.y() + rc.height() - mYVert);
}

bool BorderChecker::checkNoBorderY(QPointF const &point) const
{
	double currentX = point.x();
	QRectF rc = mNode->boundingRect();
	return (currentX >= rc.x() + mXHor) && (currentX <= rc.x() + rc.width() - mXHor);
}
