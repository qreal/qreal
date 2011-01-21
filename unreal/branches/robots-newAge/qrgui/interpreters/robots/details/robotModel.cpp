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
	qDebug() << "Initializing robot model...";
	qDebug() << "Connecting to robot...";
	mRobotCommunicationInterface->connect();
}

void RobotModel::connectedSlot()
{
	qDebug() << "Connected. Initializing touch sensor...";
	connect(&mTouchSensor, SIGNAL(configured()), this, SLOT(sensorConfigurationDoneSlot()));
	mTouchSensor.configure(0);
}

void RobotModel::sensorConfigurationDoneSlot()
{
	qDebug() << "Touch sensor configuration done";
	emit connected();
}
