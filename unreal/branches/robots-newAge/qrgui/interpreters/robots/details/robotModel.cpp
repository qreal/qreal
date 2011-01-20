#include "robotModel.h"

using namespace qReal;
using namespace interpreters::robots::details;

RobotModel::RobotModel(RobotCommunicationInterface * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
	, mBrick(robotCommunicationInterface)
	, mTouchSensor(robotCommunicationInterface)
{
	connect(mRobotCommunicationInterface, SIGNAL(connected()), this, SLOT(connectedSlot()));
}

RobotModel::~RobotModel()
{
	delete mRobotCommunicationInterface;
}

robotParts::Brick &RobotModel::brick()
{
	return mBrick;
}

robotParts::TouchSensor &RobotModel::touchSensor()
{
	return mTouchSensor;
}

void RobotModel::clear()
{
	mRobotCommunicationInterface->disconnect();
}

void RobotModel::init()
{
	mRobotCommunicationInterface->connect();
}

void RobotModel::connectedSlot()
{
	mTouchSensor.configure(0);
	emit connected();
}
