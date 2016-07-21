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

#include "scalarSensor.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Encoder on motor, returns current count of rotations of motor axis after the beginning of measurement.
class ROBOTS_KIT_BASE_EXPORT EncoderSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "encoder")
	Q_CLASSINFO("friendlyName", tr("Encoder"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	EncoderSensor(const DeviceInfo &info, const PortInfo &port);

	/// Clears encoder readings, and makes this sensor return 0, like a robot is turned on just now.
	/// @todo This name is strange.
	virtual void nullify() = 0;
};

}
}
}
