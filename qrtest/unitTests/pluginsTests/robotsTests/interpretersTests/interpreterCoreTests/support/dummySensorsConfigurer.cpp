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

#include "dummySensorsConfigurer.h"

#include <QtCore/QDebug>

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace kitBase::robotModel;

DummySensorsConfigurer::DummySensorsConfigurer(QString const &name)
	: DevicesConfigurationProvider(name)
{
}

void DummySensorsConfigurer::configureBroadly(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	deviceConfigurationChanged(robotModel, port, device, Reason::userAction);
}

DeviceInfo DummySensorsConfigurer::device(QString const &robotModel, PortInfo const &port) const
{
	return currentConfiguration(robotModel, port);
}

void DummySensorsConfigurer::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor, Reason reason)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	Q_UNUSED(reason)
	// This code can be uncommented to debug sensors configuration modifications
	// qDebug() << mName << "changed" << robotModel << port.toString() << "to" << sensor.toString() << this;
}
