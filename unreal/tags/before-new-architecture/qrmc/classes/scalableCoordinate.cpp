#include "scalableCoordinate.h"

using namespace qrmc;

ScalableCoordinate::ScalableCoordinate()
	: mValue(0), mWidth(0), mIsScalable(false)
{}

ScalableCoordinate::ScalableCoordinate(qreal value, int width, bool isScalable)
	: mValue(value), mWidth(width), mIsScalable(isScalable)
{}

qreal ScalableCoordinate::value() const
{
	return mValue;
}

bool ScalableCoordinate::isScalable() const
{
	return mIsScalable;
}

QString ScalableCoordinate::toString(bool isScaled) const
{
	qreal value = isScaled ? mValue * mWidth : mValue;
	QString result = QString::number(value);
	// uncomment this when it's supported
//	result += mIsScalable ? "a" : "%";
	return result;
}

QString ScalableCoordinate::getScalability() const
{
	if (mIsScalable)
		return "true";

	return "false";
}
