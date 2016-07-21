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

#pragma once

#include <QtCore/QScopedPointer>

#include <utils/realTimeline.h>

#include "kitBase/robotModel/robotModelInterface.h"
#include "kitBase/robotModel/configuration.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {

/// Base implementation of all kit models. Provides common functionality and default implementations for
/// RobotModelInterface methods.
class ROBOTS_KIT_BASE_EXPORT CommonRobotModel : public RobotModelInterface
{
	Q_OBJECT

public:
	CommonRobotModel(const QString &kitId, const QString &robotId);
	~CommonRobotModel() override;

	QString robotId() const;

	QString kitId() const final;

	/// Defines model initialization sequence.
	void init() final;

	/// Shall be reimplemented in models which need connection to a robot. Default implementation just
	/// emits connected().
	void connectToRobot() override;

	/// Shall be implemented in descendants to stop all robot devices. Default implementation does nothing.
	void stopRobot() override;

	/// Shall be reimplemented in models that need sometihng done to disconnect from robot. Default implementation just
	/// emits disconnected().
	void disconnectFromRobot() override;

	bool interpretedModel() const override;

	ConnectionState connectionState() const final;

	const ConfigurationInterface &configuration() const final;

	QList<PortInfo> availablePorts() const final;

	QList<PortInfo> configurablePorts() const override;

	QList<DeviceInfo> allowedDevices(const PortInfo &port) const final;

	void configureDevice(const PortInfo &port, const DeviceInfo &deviceInfo) final;

	void applyConfiguration() final;

	QHash<QString, int> buttonCodes() const override;

	/// Shall be reimplemented to provide base device classes through which devices are convertable (see description
	/// of that method in RobotModelInterface for more details). Default implementation returns empty list, so
	/// no devices are convertable.
	QList<DeviceInfo> convertibleBases() const override;

	/// Returns real-time timeline implementation. Must be redefined for 2D model-based robot models.
	utils::TimelineInterface &timeline() override;

public slots:
	/// Shall be reimplemented to update settings when user changes something on settings page. Default implementation
	/// does nothing.
	virtual void rereadSettings();

	void onInterpretationStarted() override;

protected:
	/// Adds to a model list of devices that can be connected to given port.
	void addAllowedConnection(const PortInfo &port, QList<DeviceInfo> const &devices);

	/// Restricts all devices be connected to the given port.
	void removeAllowedConnections(const PortInfo &port);

	/// Configuration that can be changed by descendants to register their devices.
	ConfigurationInterface &mutableConfiguration();

	/// Configures devices that can be configured automaticly (non-user configured, such as display and speakers)
	virtual void configureKnownDevices();

	/// Device factory. Shall be reimplemented to create concrete devices on given port by given device info.
	/// If device is unknown to a model, shall throw exception, as a list of available devices is provided by model
	/// itself by allowedDevices() method. Implementation must not take ownership on created devices, it will be taken
	/// automaticly later.
	/// @todo: see what`s with ownership
	virtual robotParts::Device *createDevice(const PortInfo &port, const DeviceInfo &deviceInfo);

private slots:
	void onConnected(bool success);

	void onDisconnected();

private:
	/// Shows which types of devices can be connected to which ports.
	/// @todo Add a notion of direction.
	QHash<PortInfo, QList<DeviceInfo>> mAllowedConnections;

	/// Devices configuration.
	Configuration mConfiguration;

	/// Model connection state.
	ConnectionState mState;

	/// True, if someone called applyConfiguration() but model was disconnected, so configuration will be applied
	/// when model becomes connected.
	bool mConfigurationPostponed;

	utils::RealTimeline mTimeline;

	/// Id of a kit in which model is defined.
	const QString mKitId;
	const QString mRobotId;
};

}
}
