#pragma once

#include <QtGui/QColor>
#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT PropertyToColor
{
public:
	static QColor propertyToColor(QString const &property);
};

}


