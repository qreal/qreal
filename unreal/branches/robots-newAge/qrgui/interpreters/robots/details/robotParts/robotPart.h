#pragma once

#include <QtCore/QObject>

#include "../../robotCommunicationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class RobotPart : public QObject
{
	Q_OBJECT

public:
	RobotPart(RobotCommunicationInterface * const robotCommunicationInterface);

protected:
	RobotCommunicationInterface *mRobotCommunicationInterface;
};

}
}
}
}
}
