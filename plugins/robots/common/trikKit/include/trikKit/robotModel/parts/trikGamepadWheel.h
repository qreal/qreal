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

#include <kitBase/robotModel/robotParts/scalarSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Sensor that reports current tilt angle of Android gamepad, if "wheel" is turned on. Max left tilt is -100,
/// max right tilt is reported as 100.
class TrikGamepadWheel: public kitBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadWheel")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Wheel"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadWheel(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}
