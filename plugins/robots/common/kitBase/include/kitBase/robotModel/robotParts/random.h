/* Copyright 2015 CyberTech Labs Ltd.
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

#include "device.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Random number generator, uses C++ implementation by default and can use random device on robot if possible and
/// desired. It is needed actually to be able to control random number generation from scripts that check exercise
/// solution.
class ROBOTS_KIT_BASE_EXPORT Random : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "random")
	Q_CLASSINFO("friendlyName", tr("Random"))
	Q_CLASSINFO("direction", "input")

public:
	Random(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Provides integer random number in a given interval.
	virtual int random(int from, int to);
};

}
}
}
