#pragma once

#include <QtCore/QObject>

#include "../../../../kernel/ids.h"

#include "robotPart.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Brick : public RobotPart
{
public:
	Brick(RobotCommunicationInterface *robotCommunicationInterface);
	void beep(unsigned time);
	void playTone(unsigned freq, unsigned time);
};

}
}
}
}
}
