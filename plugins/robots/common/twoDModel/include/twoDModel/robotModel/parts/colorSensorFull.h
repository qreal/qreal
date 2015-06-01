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

#include <kitBase/robotModel/robotParts/colorSensorFull.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the color sensor in the color recognition mode for the 2D model window.
/// Returns a recognized color enum value under this color sensor.
/// Configuration is perfomed immediately, the answer is ready immediately too.
/// @todo: Return a enum value really, not some secret hardcoded value.
class TWO_D_MODEL_EXPORT ColorSensorFull : public kitBase::robotModel::robotParts::ColorSensorFull
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (full)"))

public:
	ColorSensorFull(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
