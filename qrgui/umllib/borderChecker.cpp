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
	double const checkingPointX = point.x();
	double const checkingPointY = point.y();
	QRectF const rc = mNode->boundingRect();
	return (checkingPointX >= rc.x() + mXHor) && (checkingPointX <= rc.x() + rc.width() - mXHor) && (checkingPointY >= rc.y() + rc.height() - mYHor)
			&& (checkingPointY <= rc.y() + rc.height() + mYHor);
}

bool BorderChecker::checkUpperBorder(QPointF const &point) const
{
	double const checkingPointX = point.x();
	double const checkingPointY = point.y();
	QRectF const rc = mNode->boundingRect();
	return (checkingPointX >= rc.x() + mXHor) && (checkingPointX <= rc.x() + rc.width() - mXHor) && (checkingPointY >= rc.y() - mYHor)
			&& (checkingPointY <= rc.y() + mYHor);
}

bool BorderChecker::checkLeftBorder(QPointF const &point) const
{
	double const checkingPointX = point.x();
	double const checkingPointY = point.y();
	QRectF const rc = mNode->boundingRect();
	return (checkingPointX >= rc.x() - mXVert) && (checkingPointX <= rc.x() + mXVert) && (checkingPointY >= rc.y() + mYVert)
			&& (checkingPointY <= rc.y() + rc.height() - mYVert);
}

bool BorderChecker::checkRightBorder(QPointF const &point) const
{
	double const checkingPointX = point.x();
	double const checkingPointY = point.y();
	QRectF const rc = mNode->boundingRect();
	return (checkingPointX >= rc.x() + rc.width() - mXVert) && (checkingPointX <= rc.x() + rc.width() + mXVert) && (checkingPointY >= rc.y() + mYVert)
			&& (checkingPointY <= rc.y() + rc.height() - mYVert);
}

bool BorderChecker::checkNoBorderX(QPointF const &point) const
{
	double const checkingPointY = point.y();
	QRectF const rc = mNode->boundingRect();
	return (checkingPointY >= rc.y() + mYVert) && (checkingPointY <= rc.y() + rc.height() - mYVert);
}

bool BorderChecker::checkNoBorderY(QPointF const &point) const
{
	double const checkingPointX = point.x();
	QRectF const rc = mNode->boundingRect();
	return (checkingPointX >= rc.x() + mXHor) && (checkingPointX <= rc.x() + rc.width() - mXHor);
}
