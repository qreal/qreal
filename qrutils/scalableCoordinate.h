#pragma once

#include <QtGlobal>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT ScalableCoordinate
{
public:
	ScalableCoordinate();
	ScalableCoordinate(qreal value, bool isScalable);
	qreal value() const;
	bool isScalable() const;

private:
	qreal mValue;
	bool mIsScalable;
};
}
