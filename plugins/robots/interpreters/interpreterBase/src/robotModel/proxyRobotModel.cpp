#include "interpreterBase/robotModel/proxyRobotModel.h"

using namespace interpreterBase::robotModel;

void ProxyRobotModel::init()
{
	mProxiedModel->init();
}

void ProxyRobotModel::stopRobot()
{
	mProxiedModel->stopRobot();
}

void ProxyRobotModel::disconnectFromRobot()
{
	mProxiedModel->disconnectFromRobot();
}

bool ProxyRobotModel::needsConnection() const
{
	return mProxiedModel->needsConnection();
}

ConfigurationInterface &ProxyRobotModel::configuration()
{
	return mProxiedModel->configuration();
}

robotParts::Brick &ProxyRobotModel::brick()
{
	return mProxiedModel->brick();
}

robotParts::Display &ProxyRobotModel::display()
{
	return mProxiedModel->display();
}
