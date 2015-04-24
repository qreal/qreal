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

#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <kitBase/robotModel/configuration.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelManagerInterfaceMock : public  kitBase::robotModel::RobotModelManagerInterface
{
	Q_OBJECT

public:
	MOCK_CONST_METHOD0(model,  kitBase::robotModel::RobotModelInterface &());

	void emitConnected() {
		emit connected(true, "");
	}

	void emitAllDevicesConfigured() {
		emit allDevicesConfigured();
	}

	void emitDisconnected() {
		emit disconnected();
	}
};

}
