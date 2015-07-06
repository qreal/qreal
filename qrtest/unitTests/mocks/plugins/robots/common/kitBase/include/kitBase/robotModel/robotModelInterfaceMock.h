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

#include <kitBase/robotModel/robotModelInterface.h>
#include <kitBase/robotModel/configuration.h>
#include <utils/timelineInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

typedef QHash<QString, int> StringIntHash;

class RobotModelInterfaceMock : public kitBase::robotModel::RobotModelInterface
{
	Q_OBJECT

public:
	MOCK_CONST_METHOD0(robotId, QString());
	MOCK_CONST_METHOD0(name, QString());
	MOCK_CONST_METHOD0(friendlyName, QString());
	MOCK_CONST_METHOD0(kitId, QString());
	MOCK_CONST_METHOD0(priority, int());
	MOCK_CONST_METHOD0(interpretedModel, bool());

	MOCK_METHOD0(init, void());

	MOCK_METHOD0(connectToRobot, void());
	MOCK_METHOD0(stopRobot, void());
	MOCK_METHOD0(disconnectFromRobot, void());

	MOCK_CONST_METHOD0(connectionState, ConnectionState());

	MOCK_CONST_METHOD0(needsConnection, bool());

	MOCK_CONST_METHOD0(configuration
			, kitBase::robotModel::ConfigurationInterface const &());
	MOCK_METHOD0(mutableConfiguration,  kitBase::robotModel::ConfigurationInterface&());

	MOCK_CONST_METHOD0(availablePorts, QList< kitBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD0(buttonCodes, StringIntHash());
	MOCK_CONST_METHOD0(configurablePorts, QList< kitBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD1(allowedDevices, QList< kitBase::robotModel::DeviceInfo>(
			kitBase::robotModel::PortInfo const &port));

	MOCK_METHOD2(configureDevice, void( kitBase::robotModel::PortInfo const & port
			, kitBase::robotModel::DeviceInfo const &deviceInfo));

	MOCK_METHOD0(applyConfiguration, void());

	MOCK_CONST_METHOD0(convertibleBases, QList< kitBase::robotModel::DeviceInfo>());

	MOCK_METHOD0(timeline, utils::TimelineInterface &());

	MOCK_METHOD0(onInterpretationStarted, void());
};

}
