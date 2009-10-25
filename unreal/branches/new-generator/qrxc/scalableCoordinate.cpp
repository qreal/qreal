#include "scalableCoordinate.h"

ScalableCoordinate::ScalableCoordinate()
	: mValue(0), mIsScalable(false)
{}

ScalableCoordinate::ScalableCoordinate(qreal value, bool isScalable)
	: mValue(value), mIsScalable(isScalable)
{}

qreal ScalableCoordinate::value()
{
	return mValue;
}

bool ScalableCoordinate::isScalable()
{
	return mIsScalable;
}