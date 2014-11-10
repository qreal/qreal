#include "interpreterBase/robotModel/commonRobotModel.h"

#include <qrkernel/exception/exception.h>
#include <utils/realTimeline.h>

#include "interpreterBase/robotModel/robotParts/motor.h"

#include <QtCore/QDebug>

using namespace interpreterBase::robotModel;

CommonRobotModel::CommonRobotModel(QString const &kitId, QString const &robotId)
	: mState(disconnectedState)
	, mConfigurationPostponed(false)
	, mKitId(kitId)
	, mRobotId(robotId)
{
	connect(&mConfiguration, &Configuration::allDevicesConfigured
			, this, &CommonRobotModel::allDevicesConfigured, Qt::QueuedConnection);

	connect(this, &CommonRobotModel::connected, this, &CommonRobotModel::onConnected);
	connect(this, &CommonRobotModel::disconnected, this, &CommonRobotModel::onDisconnected);
}

CommonRobotModel::~CommonRobotModel()
{
	stopRobot();
}

QString CommonRobotModel::robotId() const
{
	return mRobotId;
}

QString CommonRobotModel::kitId() const
{
	return mKitId;
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
	for (robotParts::Device * const device : mConfiguration.devices()) {
		robotParts::Motor * const motor = dynamic_cast<robotParts::Motor *>(device);
		if (motor) {
			motor->off();
		}
	}
	/// @todo: add known deinitialization methods here (for example sensors termination after extending their inteface)
}

void CommonRobotModel::disconnectFromRobot()
{
	emit disconnected();
}

CommonRobotModel::ConnectionState CommonRobotModel::connectionState() const
{
	return needsConnection() ? mState : connectedState;
}

ConfigurationInterface &CommonRobotModel::mutableConfiguration()
{
	return mConfiguration;
}

void CommonRobotModel::configureKnownDevices()
{
	QSet<PortInfo> const nonConfigurablePorts = availablePorts().toSet() - configurablePorts().toSet();
	for (PortInfo const &port : nonConfigurablePorts.toList()) {
		for (DeviceInfo const &device : allowedDevices(port)) {
			configureDevice(port, device);
		}
	}
}

void CommonRobotModel::onConnected(bool success)
{
	if (success) {
		mState = connectedState;

		if (mConfigurationPostponed) {
			mConfiguration.applyConfiguration();
			mConfigurationPostponed = false;
		}
	} else {
		mState = disconnectedState;
	}
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
	QList<PortInfo> result = mAllowedConnections.keys();
	qSort(result);
	return result;
}

QList<PortInfo> CommonRobotModel::configurablePorts() const
{
	QList<PortInfo> result;

	for (PortInfo const &port : availablePorts()) {
		QList<DeviceInfo> const devices = allowedDevices(port);
		if (devices.empty()) {
			/// @todo: Display error?
		}

		// Device can be automaticly configured if it is the only one that can be plugged into this port
		// (for example display) or if two devices with different directions can be plugged into this port
		// (fer example motor and encoder). Otherwise this device must be configured manually by user.
		if (devices.count() > 1) {
			result << port;
		}
	}

	return result;
}

QList<DeviceInfo> CommonRobotModel::allowedDevices(PortInfo const &port) const
{
	return mAllowedConnections[port];
}

void CommonRobotModel::configureDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (!availablePorts().contains(port)) {
		return;
	}

	if (deviceInfo.isNull()) {
		mConfiguration.clearDevice(port);
		return;
	}

	robotParts::Device * const device = createDevice(port, deviceInfo);
	if (device) {
		mConfiguration.configureDevice(device);
	} else {
		qDebug() << "Can not create device for " << deviceInfo.toString();
	}
	/// @todo Handle error
}

void CommonRobotModel::applyConfiguration()
{
	if (connectionState() == connectedState) {
		mConfiguration.applyConfiguration();
	} else {
		mConfigurationPostponed = true;
	}
}

QList<DeviceInfo> CommonRobotModel::convertibleBases() const
{
	return {};
}

utils::TimelineInterface &CommonRobotModel::timeline()
{
	return mTimeline;
}

void CommonRobotModel::rereadSettings()
{
}

void CommonRobotModel::onInterpretationStarted()
{
}

void CommonRobotModel::addAllowedConnection(PortInfo const &port, QList<DeviceInfo> const &devices)
{
	mAllowedConnections[port].append(devices);
}

robotParts::Device * CommonRobotModel::createDevice(
		PortInfo const &port, DeviceInfo const &deviceInfo)
{
	Q_UNUSED(port);
	Q_UNUSED(deviceInfo);

	/// @todo Handle error?
	return nullptr;
}
