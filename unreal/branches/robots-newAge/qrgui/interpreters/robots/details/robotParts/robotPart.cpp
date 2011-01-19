#include "robotPart.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

RobotPart::RobotPart(RobotCommunicationInterface * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
{
}
