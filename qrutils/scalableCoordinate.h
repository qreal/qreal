#pragma once

#include <QtGlobal>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT ScalableCoordinate
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
}
