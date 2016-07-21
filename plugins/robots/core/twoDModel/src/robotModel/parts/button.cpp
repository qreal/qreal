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

#include "twoDModel/robotModel/parts/button.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"
#include "twoDModel/engine/twoDModelDisplayWidget.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

Button::Button(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, int code
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::Button(info, port, code)
	, mEngine(engine)
{
}

void Button::read()
{
	emit newData(mEngine.display()->buttonIsDown(port().name()) ? 1 : 0);
}
