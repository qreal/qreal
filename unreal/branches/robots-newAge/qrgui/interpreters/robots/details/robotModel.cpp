#include "robotModel.h"

using namespace qReal;
using namespace interpreters::robots::details;

RobotModel::RobotModel(RobotCommunicationInterface * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
	, mBrick(robotCommunicationInterface)
{
}

RobotModel::~RobotModel()
{
	delete mRobotCommunicationInterface;
}

robotParts::Brick &RobotModel::brick()
{
	return mBrick;
}

void RobotModel::clear()
{
	mRobotCommunicationInterface->disconnect();
}

void RobotModel::init()
{
	mRobotCommunicationInterface->connect(6);
}
