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

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "kitBase/robotModel/portInfo.h"
#include "kitBase/robotModel/deviceInfo.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Anything that can be plugged into a port on a robot. Bound to a port at creation time and shall be configured
/// before it becomes ready to work. Base class for all engines/sensors/displays and so on.
/// Each concrete device should have Q_CLASSINFO macroses for friendlyName and device direction.
///
/// @todo Most subclasses are actually just tags for inheritance hierarchy, but due to C++ syntax it takes about 35
///       lines of code to define each. We shall consider usage of internal DSL (macros? templates?) or even
///       visual DSL on QReal for device declarations. Or rewrite everything on F#.
class ROBOTS_KIT_BASE_EXPORT Device : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param port - port on which this device shall be configured.
	Device(const DeviceInfo &info, const PortInfo &port);

	/// Returns port on which this device is or shall be configured.
	const PortInfo &port() const;

	/// Returns device type for non-abstract (physical) device.
	virtual const DeviceInfo &deviceInfo() const;

	/// Returns true if device is ready for work (i.e. configured).
	bool ready() const;

	/// Configure device.
	void configure();

signals:
	/// Emitted when device finished its configuration, successfully or by failure. Note that configuration can be
	/// synchronous or asynchronous for various devices (configure() can be in a stack when this signal is emitted).
	/// Shall not be emitted directly from descendants, use configurationCompleted() instead due to timeout handling
	/// performed by Device class.
	void configured(bool success);

	/// Emitted when of the value of device`s \a property has changed to \a value.
	/// The device itself may notify other components about its inner state via this signal.
	void propertyChanged(const QString &property, const QVariant &value);

protected:
	/// Perform actual configuration of a device. Shall be implemented for concrete devices with non-trivial
	/// configuration logic default implementation tells that device is already configured.
	virtual void doConfiguration();

	/// Concrete devices shall call this method when they finish their configuration.
	void configurationCompleted(bool success);

private slots:
	/// Called when configuration takes more than timeout time. Note that if configuration is done synchronously,
	/// timer signal will never be emitted and this slot will not be called.
	void configurationTimerTimeoutSlot();

private:
	const DeviceInfo mInfo;
	const PortInfo mPort;
	QTimer mConfigurationTimeoutTimer;
};

}
}
}
