#include "interpreterBase/robotModel/proxyRobotModel.h"

using namespace interpreterBase::robotModel;

RobotModelInterface *ProxyRobotModel::proxiedModel() const
{
	return mProxiedModel;
}

void ProxyRobotModel::setProxiedModel(RobotModelInterface * const robotModel)
{
	mProxiedModel = robotModel;
}

QString ProxyRobotModel::name() const
{
	return mProxiedModel->name();
}

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

ConfigurationInterface &ProxyRobotModel::mutableConfiguration()
{
	return mProxiedModel->mutableConfiguration();
}

ConfigurationInterface const &ProxyRobotModel::configuration() const
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

QList<PortInfo> ProxyRobotModel::availablePorts() const
{
	return mProxiedModel->availablePorts();
}

QList<PluggableDeviceInfo> ProxyRobotModel::supportedSensors() const
{
	return mProxiedModel->supportedSensors();
}
