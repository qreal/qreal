#pragma once

#include "../../../qrkernel/exception/exception.h"

namespace robotsInterpreter {
namespace robotCommunication {

class RobotCommunicationException : public qReal::Exception
{
	public:
		explicit RobotCommunicationException(QString const &message);
};

}
}
