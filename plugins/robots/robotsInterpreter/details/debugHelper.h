#pragma once

#include <QtCore/QString>

#include "../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class DebugHelper
{
public:
	static QString toString(robotModelType::robotModelTypeEnum const &modelType);
};

}
}
}
}
