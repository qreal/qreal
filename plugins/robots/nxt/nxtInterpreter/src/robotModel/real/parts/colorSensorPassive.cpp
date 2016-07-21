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

#include "colorSensorPassive.h"

using namespace nxt::robotModel::real::parts;

ColorSensorPassive::ColorSensorPassive(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorPassive(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORNONE)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorPassive::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorPassive::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorPassive::failure);
}

void ColorSensorPassive::read()
{
	mImpl.read();
}

void ColorSensorPassive::doConfiguration()
{
	mImpl.doConfiguration();
}
