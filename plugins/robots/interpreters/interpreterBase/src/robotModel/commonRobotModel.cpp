#include "interpreterBase/robotModel/commonRobotModel.h"

#include <qrkernel/exception/exception.h>

#include "interpreterBase/robotModel/robotParts/touchSensor.h"

using namespace interpreterBase::robotModel;

CommonRobotModel::CommonRobotModel()
{
	connect(&mConfiguration, &Configuration::allDevicesConfigured
			, this, &CommonRobotModel::allDevicesConfigured);

	connect(this, &CommonRobotModel::connected, this, &CommonRobotModel::onConnected);
	connect(this, &CommonRobotModel::disconnected, this, &CommonRobotModel::onDisconnected);
}

CommonRobotModel::~CommonRobotModel()
{
	stopRobot();
}

void CommonRobotModel::init()
{
	rereadSettings();
	configureKnownDevices();
}

void CommonRobotModel::connectToRobot()
{
	emit connected(true);
}

void CommonRobotModel::stopRobot()
{
}

void CommonRobotModel::disconnectFromRobot()
{
	emit disconnected();
}

ConfigurationInterface &CommonRobotModel::mutableConfiguration()
{
	return mConfiguration;
}

void CommonRobotModel::onConnected(bool success)
{
	mState = connectedState;
}

void CommonRobotModel::onDisconnected()
{
	mState = disconnectedState;
}

ConfigurationInterface const &CommonRobotModel::configuration() const
{
	return mConfiguration;
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

void CommonRobotModel::configureDevices(QHash<PortInfo, PluggableDeviceInfo> const &devices)
{
	mConfiguration.lockConfiguring();

	for (PortInfo const &port : devices.keys()) {
		configureDevice(port, devices.value(port));
	}

	mConfiguration.unlockConfiguring();
}

void CommonRobotModel::configureDevice(PortInfo const &port, PluggableDeviceInfo const &deviceInfo)
{
	robotParts::PluggableDevice *device = createDevice(port, deviceInfo);
	if (device) {
		mConfiguration.configureDevice(device);
	}
	/// @todo Handle error
}

QList<PluggableDeviceInfo> CommonRobotModel::convertibleBases() const
{
	return {};
}

void CommonRobotModel::rereadSettings()
{
}

void CommonRobotModel::addAllowedConnection(PortInfo const &port, QList<PluggableDeviceInfo> const &devices)
{
	mAllowedConnections[port].append(devices);
}

void CommonRobotModel::configureKnownDevices()
{
}

robotParts::PluggableDevice * CommonRobotModel::createDevice(
		PortInfo const &port, PluggableDeviceInfo const &deviceInfo)
{
	Q_UNUSED(port);
	Q_UNUSED(deviceInfo);

	/// @todo Handle error?
	return nullptr;
}
