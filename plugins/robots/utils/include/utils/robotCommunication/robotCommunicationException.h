#pragma once

#include <qrkernel/exception/exception.h>

#include "utils/utilsDeclSpec.h"

namespace utils {
namespace robotCommunication {

/// Thrown when something went wrong during communication with robot
class ROBOTS_UTILS_EXPORT RobotCommunicationException : public qReal::Exception
{
public:
	explicit RobotCommunicationException(QString const &message);
};

}
}
