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

#include "colorSensorRed.h"

using namespace nxt::robotModel::real::parts;

ColorSensorRed::ColorSensorRed(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorRed(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORRED)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorRed::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorRed::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorRed::failure);
}

void ColorSensorRed::read()
{
	mImpl.read();
}

void ColorSensorRed::doConfiguration()
{
	mImpl.doConfiguration();
}
