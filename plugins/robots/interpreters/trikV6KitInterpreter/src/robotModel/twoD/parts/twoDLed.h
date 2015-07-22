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

#include <trikKit/robotModel/parts/trikLed.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDLed : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	TwoDLed(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	/// sets LED widget's color in 2D model.
	void setColor(const QString &color) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}
