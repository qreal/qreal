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

#include <kitBase/robotModel/robotParts/abstractSensor.h>

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Sensor that returns single integer value.
class ROBOTS_KIT_BASE_EXPORT ScalarSensor : public AbstractSensor
{
	Q_OBJECT
	Q_PROPERTY(int value READ lastData WRITE setLastData)

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ScalarSensor(const DeviceInfo &info, const PortInfo &port);

	/// Returns last value passed to 'newData' signal parameter, i.e. the last obtained value from the sensor.
	int lastData() const;

	/// Forces system think that last returned by sensor value was the given one.
	void setLastData(int data);

signals:
	/// Emitted when new data is ready. Note that concrete sensors may work in "pull" and in "push" mode. In "pull"
	/// mode this signal is emitted when sensor reads data after "read" call, in "push" mode this signal is emitted
	/// every time new data is ready, regardless of "read" calls.
	void newData(int reading);

private:
	int mLastValue;
};

}
}
}
