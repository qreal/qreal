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

#include "colorSensorFull.h"

using namespace nxt::robotModel::real::parts;

ColorSensorFull::ColorSensorFull(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorFull(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORFULL)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorFull::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorFull::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorFull::failure);
}

void ColorSensorFull::read()
{
	mImpl.read();
}

void ColorSensorFull::doConfiguration()
{
	mImpl.doConfiguration();
}
