/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "kitBase/robotModel/commonRobotModel.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>
#include <utils/realTimeline.h>

#include "kitBase/robotModel/robotParts/motor.h"
#include "kitBase/robotModel/robotParts/random.h"

using namespace kitBase::robotModel;

CommonRobotModel::CommonRobotModel(const QString &kitId, const QString &robotId)
	: mState(disconnectedState)
	, mConfigurationPostponed(false)
	, mKitId(kitId)
	, mRobotId(robotId)
{
	connect(&mConfiguration, &Configuration::allDevicesConfigured
			, this, &CommonRobotModel::allDevicesConfigured, Qt::QueuedConnection);

	connect(this, &CommonRobotModel::connected, this, &CommonRobotModel::onConnected);
	connect(this, &CommonRobotModel::disconnected, this, &CommonRobotModel::onDisconnected);

	addAllowedConnection(PortInfo("RandomPort", input), { DeviceInfo::create<robotParts::Random>() });
}

CommonRobotModel::~CommonRobotModel()
{
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
	emit connected(true, QString());
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

bool CommonRobotModel::interpretedModel() const
{
	return true;
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
	for (const PortInfo &port : nonConfigurablePorts.toList()) {
		for (const DeviceInfo &device : allowedDevices(port)) {
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

const ConfigurationInterface &CommonRobotModel::configuration() const
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

	for (const PortInfo &port : availablePorts()) {
		QList<DeviceInfo> const devices = allowedDevices(port);
		if (devices.empty()) {
			/// @todo: Display error?
		}

		// Device can be automaticly configured if it is the only one that can be plugged into this port
		// (for example display) or if two devices with different directions can be plugged into this port
		// (for example motor and encoder). Otherwise this device must be configured manually by user.
		if (devices.count() > 1) {
			result << port;
		}
	}

	return result;
}

QList<DeviceInfo> CommonRobotModel::allowedDevices(const PortInfo &port) const
{
	return mAllowedConnections[port];
}

void CommonRobotModel::configureDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
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
		QLOG_WARN() << "Can not create device for" << deviceInfo.toString();
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

QHash<QString, int> CommonRobotModel::buttonCodes() const
{
	return QHash<QString, int>();
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

void CommonRobotModel::addAllowedConnection(const PortInfo &port, QList<DeviceInfo> const &devices)
{
	mAllowedConnections[port].append(devices);
}

void CommonRobotModel::removeAllowedConnections(const PortInfo &port)
{
	mAllowedConnections.remove(port);
}

robotParts::Device * CommonRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Random>()) {
		return new robotParts::Random(deviceInfo, port);
	}

	/// @todo Handle error?
	return nullptr;
}
