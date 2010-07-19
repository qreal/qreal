#pragma once

#include <QtGlobal>

class ScalableCoordinate
{
public:
	ScalableCoordinate();
	ScalableCoordinate(qreal value, bool isScalable);
	qreal value();
	bool isScalable();

private:
	qreal mValue;
	bool mIsScalable;
};
