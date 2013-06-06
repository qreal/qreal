#include "scalableCoordinate.h"

using namespace utils;

ScalableCoordinate::ScalableCoordinate()
	: mValue(0), mIsScalable(false)
{}

ScalableCoordinate::ScalableCoordinate(qreal value, bool isScalable)
	: mValue(value), mIsScalable(isScalable)
{}

qreal ScalableCoordinate::value() const
{
	return mValue;
}

bool ScalableCoordinate::isScalable() const
{
	return mIsScalable;
}
