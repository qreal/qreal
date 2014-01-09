#pragma once

#include "../../../qrkernel/exception/exception.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotCommunicationException : public qReal::Exception
{
	public:
		explicit RobotCommunicationException(QString const &message);
};

}
}
}
}
