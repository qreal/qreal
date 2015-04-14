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

#include "colorSensorBlue.h"

using namespace nxt::robotModel::real::parts;

ColorSensorBlue::ColorSensorBlue(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorBlue(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORBLUE)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorBlue::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorBlue::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorBlue::failure);
}

void ColorSensorBlue::read()
{
	mImpl.read();
}

void ColorSensorBlue::doConfiguration()
{
	mImpl.doConfiguration();
}
