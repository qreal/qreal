/* Copyright 2016 Yurii Litvinov
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

#include <utils/robotCommunication/tcpRobotCommunicatorInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class TcpRobotCommunicatorMock : public utils::robotCommunication::TcpRobotCommunicatorInterface
{
public:
	MOCK_METHOD1(uploadProgram, void(const QString &));
	MOCK_METHOD1(runProgram, void(const QString &));
	MOCK_METHOD2(runDirectCommand, void(const QString &, bool));
	MOCK_METHOD0(requestCasingVersion, void());
	MOCK_METHOD0(stopRobot, void());
	MOCK_METHOD1(requestData, void(const QString &));
	MOCK_METHOD0(requestData, void());
	MOCK_METHOD0(connect, void());
	MOCK_METHOD0(disconnect, void());
};

}
