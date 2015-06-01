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

#include <kitBase/robotModel/robotParts/vectorSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Device representing TRIK camera object detector.
class TrikObjectSensor : public kitBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "trikObjectSensor")
	Q_CLASSINFO("friendlyName", tr("Object Sensor"))

public:
	TrikObjectSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Turns camera on and prepares a sensor.
	virtual void init() = 0;

	/// Locks dominant color at center of a field of view of a camera as object color and begins tracking it.
	virtual void detect() = 0;
};

}
}
}
