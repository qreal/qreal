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

#include "colorSensorGreen.h"

using namespace nxt::robotModel::real::parts;

ColorSensorGreen::ColorSensorGreen(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorGreen(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORGREEN)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorGreen::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorGreen::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorGreen::failure);
}

void ColorSensorGreen::read()
{
	mImpl.read();
}

void ColorSensorGreen::doConfiguration()
{
	mImpl.doConfiguration();
}
