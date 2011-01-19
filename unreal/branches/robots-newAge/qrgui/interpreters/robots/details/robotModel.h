#pragma once

#include "robotParts/brick.h"
#include "../robotCommunicationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotModel {
public:
	RobotModel(RobotCommunicationInterface * const robotCommunicationInterface);  // Gets ownership
	~RobotModel();
	robotParts::Brick &brick();
	void init();
	void clear();

private:
	RobotCommunicationInterface *mRobotCommunicationInterface;  // Has ownership
	robotParts::Brick mBrick;
};

}
}
}
}
