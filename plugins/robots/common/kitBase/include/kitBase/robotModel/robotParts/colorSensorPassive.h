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

/// Base class for sensor that returns intensity of white under it, so it works as generic light sensor. It is needed
/// because NXT uses it as one of the modes of color sensor.
/// @todo Rewrite 2D model and this stuff to avoid NXT dependencies. We shall have our own means to simulate complex
///       sensors like NXT color.
class ROBOTS_KIT_BASE_EXPORT ColorSensorPassive : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorNone")
	Q_CLASSINFO("friendlyName", tr("Color sensor (passive)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorPassive(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
