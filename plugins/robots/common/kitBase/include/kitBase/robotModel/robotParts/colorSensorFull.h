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

#include "colorSensor.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns a code of color under it. Following NXT specification it is recommended to use
/// following color codes:
/// black - 1, blue - 2, green - 3, yellow - 4, red - 5, white - 6, cyan - 7, magenta - 8.
/// Other kits may provide their own color values, and must define their own blocks to work with them.
class ROBOTS_KIT_BASE_EXPORT ColorSensorFull : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorRecognition")
	Q_CLASSINFO("friendlyName", tr("Color sensor (full)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorFull(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
