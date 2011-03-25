#pragma once

#include <QtCore/QString>
#include "scalableCoordinate.h"

class ScalableItem
{
protected:
	static ScalableCoordinate initCoordinate(QString const &coordinate, int maxValue);
	virtual ~ScalableItem() {};
};
