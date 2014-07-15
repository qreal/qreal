#pragma once

#include <QtGui/QColor>

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT Utils
{
public:

	/// I'ts function return QColor depending in user choice.
	/// By default return gray.
	static QColor propertyToColor(QString const &property);
};

}
