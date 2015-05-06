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

#include <kitBase/devicesConfigurationProvider.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummySensorsConfigurer : public kitBase::DevicesConfigurationProvider
{
public:
	explicit DummySensorsConfigurer(QString const &name = QString());

	/// Configures given device with broadcasting changes to the whole graph
	void configureBroadly(QString const &robotModel
			, kitBase::robotModel::PortInfo const &port
			, kitBase::robotModel::DeviceInfo const &device);

	kitBase::robotModel::DeviceInfo device(QString const &robotModel
			, kitBase::robotModel::PortInfo const &port) const;

protected:
	void onDeviceConfigurationChanged(QString const &robotModel
			, kitBase::robotModel::PortInfo const &port
			, kitBase::robotModel::DeviceInfo const &sensor
			, kitBase::DevicesConfigurationProvider::Reason reason) override;
};

}
}
}
