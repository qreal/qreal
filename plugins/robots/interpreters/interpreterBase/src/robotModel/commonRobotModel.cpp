#include "interpreterBase/robotModel/commonRobotModel.h"

using namespace interpreterBase::robotModel;

CommonRobotModel::CommonRobotModel()
{
}

CommonRobotModel::~CommonRobotModel()
{
}

void CommonRobotModel::init()
{
	/// @todo Remove this sh~.
	emit connected(true);
}

void CommonRobotModel::stopRobot()
{
}

void CommonRobotModel::disconnectFromRobot()
{
}

ConfigurationInterface &CommonRobotModel::mutableConfiguration()
{
	return mConfiguration;
}

ConfigurationInterface const &CommonRobotModel::configuration() const
{
	return mConfiguration;
}

robotParts::Brick &CommonRobotModel::brick()
{
	return mBrick;
}

robotParts::Display &CommonRobotModel::display()
{
	return mDisplay;
}

QList<PortInfo> CommonRobotModel::availablePorts() const
{
	return mAllowedConnections.keys();
}

QList<PortInfo> CommonRobotModel::configurablePorts() const
{
	return availablePorts();
}

QList<PluggableDeviceInfo> CommonRobotModel::allowedDevices(PortInfo const &port) const
{
	return mAllowedConnections[port];
}
