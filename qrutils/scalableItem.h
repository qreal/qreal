#pragma once

#include <QtCore/QString>
#include "scalableCoordinate.h"
#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT ScalableItem
{
public:
	static ScalableCoordinate initCoordinate(const QString &coordinate, int maxValue);
protected:
	virtual ~ScalableItem() {}
};

}
