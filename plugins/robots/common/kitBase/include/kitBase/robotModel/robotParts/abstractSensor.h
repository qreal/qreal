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

#include <kitBase/robotModel/robotParts/device.h>
#include <kitBase/kitBaseDeclSpec.h>

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Abstract sensor device which can be asked to provide readings. Signals with readings are defined in subclasses
/// since readings can have different types, like scalars for touch or range sensor, or vector for accelerometer.
/// Sensors can also work in "push" mode, providing values as soon as they are ready, in that case "read()" call is
/// ignored.
/// General workflow is to subscribe to "newData" and "failure" signals and call read().
class ROBOTS_KIT_BASE_EXPORT AbstractSensor : public Device
{
	Q_OBJECT
	Q_CLASSINFO("direction", "input")
	Q_PROPERTY(bool locked READ isLocked WRITE setLocked)

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	AbstractSensor(const DeviceInfo &info, const PortInfo &port);

	/// Ask sensor to provide new readings. Returns immediately. When reading is done, sensor emits "newData" signal
	/// with reading as a parameter.
	/// This call is ignored if sensor works in a "push" mode and always provides readings as soon as they are ready.
	/// If more than one call of "read" was made before sensor returned values, subsequent calls are ignored.
	virtual void read() = 0;

	/// Returns true if sensor state was locked for writing. That means that last obtained sensor value
	/// will not be refreshed until setLocked(true) will be called.
	bool isLocked() const;

	/// Locks or unlocks sensor state for writing. If true is passed last obtained sensor value will be frozen.
	/// If false values will be refreshed again. Ideologically lock must be removed before interpretation starts.
	void setLocked(bool locked);

signals:
	/// Emitted when sensor reading is failed for some reason. Note that it is possible that sensor will not emit any
	/// signal in case of a failure, so if "failure()" is emitted, the sensor is failed, but if not, sensor may be still
	/// preparing readings and it can take some time, or simply failed silently.
	void failure();

private:
	bool mIsLocked;
};

}
}
}
