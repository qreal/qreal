/* Copyright 2015 CyberTech Labs Ltd., Yurii Litvinov
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

#include <kitBase/robotModel/portInfo.h>
#include <kitBase/robotModel/robotParts/device.h>

#include "trikGeneratorBase/trikGeneratorBaseDeclSpec.h"

namespace trik {
namespace robotModel {

class ROBOTS_TRIK_GENERATOR_BASE_EXPORT GeneratorModelExtensionInterface
{
public:
	virtual ~GeneratorModelExtensionInterface() {}

	/// Adds pre-configured device to the given port.
	virtual void addDevice(const kitBase::robotModel::PortInfo &port
			, kitBase::robotModel::robotParts::Device * const device) = 0;
};

}
}
