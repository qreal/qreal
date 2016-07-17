/* Copyright 2016 CyberTech Labs Ltd.
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

#include "createRemoveSensorImplementation.h"

#include "twoDModel/engine/model/sensorsConfiguration.h"

using namespace twoDModel;
using namespace commands;

CreateRemoveSensorImplementation::CreateRemoveSensorImplementation(model::SensorsConfiguration &configurator
		, const QString &robotModel
		, const kitBase::robotModel::PortInfo &port
		, const kitBase::robotModel::DeviceInfo &device
		, const QPointF &position
		, const qreal direction)
	: mConfigurator(configurator)
	, mRobotModel(robotModel)
	, mPort(port)
	, mDevice(device)
	, mPosition(position)
	, mDirection(direction)
{
	connectDevicesConfigurationProvider(&mConfigurator);
}

void CreateRemoveSensorImplementation::create()
{
	deviceConfigurationChanged(mRobotModel, mPort, mDevice, Reason::userAction);
	mConfigurator.setPosition(mPort, mPosition);
	mConfigurator.setDirection(mPort, mDirection);
}

void CreateRemoveSensorImplementation::remove()
{
	mDevice = mConfigurator.type(mPort);
	mPosition = mConfigurator.position(mPort);
	mDirection = mConfigurator.direction(mPort);
	deviceConfigurationChanged(mRobotModel, mPort, kitBase::robotModel::DeviceInfo(), Reason::userAction);
}
