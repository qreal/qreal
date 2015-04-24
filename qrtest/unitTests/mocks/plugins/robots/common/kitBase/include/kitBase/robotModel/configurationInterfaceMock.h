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

#include <kitBase/robotModel/configurationInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ConfigurationInterfaceMock : public  kitBase::robotModel::ConfigurationInterface
{
public:
	MOCK_METHOD1(configureDevice, void( kitBase::robotModel::robotParts::Device * const device));

	MOCK_CONST_METHOD1(device,  kitBase::robotModel::robotParts::Device *(
			 kitBase::robotModel::PortInfo const &port));

	MOCK_CONST_METHOD0(devices, QList< kitBase::robotModel::robotParts::Device *>());

	MOCK_METHOD1(clearDevice, void( kitBase::robotModel::PortInfo const &port));
	MOCK_METHOD0(applyConfiguration, void());
};

}
