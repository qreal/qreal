#pragma once
#include "abstractBrickImplementation.h"
#include "../../../robotCommunicationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace brickImplementations {

class RealBrickImplementation : public AbstractBrickImplementation
{
	Q_OBJECT
public:
	RealBrickImplementation(RobotCommunicationInterface * const robotCommunicationInterface);
	virtual void beep(unsigned time);
	virtual void playTone(unsigned freq, unsigned time);
private:
	RobotCommunicationInterface * const mRobotCommunicationInterface;
};

}
}
}
}
}
}
