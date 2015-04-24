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

#include <kitBase/robotModel/robotParts/display.h>

namespace ev3 {
namespace robotModel {
namespace parts {

class Ev3Display : public kitBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	Ev3Display(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port);

	virtual void drawCircle(int x, int y, int radius, bool filled) = 0;
	virtual void drawRect(int x, int y, int width, int height, bool filled) = 0;
};

}
}
}
