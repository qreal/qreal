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

#include "runProgramProtocolTest.h"

#include <utils/robotCommunication/runProgramProtocol.h>

#include <QtCore/QFileInfo>

#include <testUtils/signalsTester.h>
#include <testUtils/delay.h>

#include "support/tcpRobotCommunicatorMock.h"

using namespace utils::robotCommunication;
using namespace qrTest;
using namespace qrTest::robotsTests::utilsTests;
using namespace ::testing;

void RunProgramProtocolTest::SetUp()
{
	mCommunicator.reset(new TcpRobotCommunicatorMock{});

	ON_CALL(*mCommunicator, requestCasingVersion()).WillByDefault(
		Invoke([this]() {
			delay([this](){ emit mCommunicator->casingVersionReceived("model-2015"); });
		})
	);

	ON_CALL(*mCommunicator, uploadProgram(_)).WillByDefault(
		Invoke([this](const QString &name) {
			Q_UNUSED(name)
			delay([this]() { emit mCommunicator->uploadProgramDone(); });
		})
	);

	ON_CALL(*mCommunicator, runProgram(_)).WillByDefault(
		Invoke([this](const QString &name) {
			Q_UNUSED(name)
			delay([this]() { emit mCommunicator->startedRunning(); });
		})
	);
}

TEST_F(RunProgramProtocolTest, mainExecutionTest)
{
	SignalsTester signalsTester{};

	const QString configVersion = "model-2015";

	EXPECT_CALL(*mCommunicator, requestCasingVersion()).Times(1);
	EXPECT_CALL(*mCommunicator, uploadProgram(_)).Times(1);
	EXPECT_CALL(*mCommunicator, runProgram(_)).Times(1);

	RunProgramProtocol protocol(*mCommunicator, configVersion);

	signalsTester.expectSignal(&protocol, &RunProgramProtocol::success, "success");

	protocol.run(QFileInfo("test"));

	signalsTester.wait(250);

	ASSERT_TRUE(signalsTester.allIsGood());
}

TEST_F(RunProgramProtocolTest, incorrectCasingVersion)
{
	SignalsTester signalsTester{};

	const QString configVersion = "model-2014";

	EXPECT_CALL(*mCommunicator, requestCasingVersion()).Times(1);
	EXPECT_CALL(*mCommunicator, uploadProgram(_)).Times(0);
	EXPECT_CALL(*mCommunicator, runProgram(_)).Times(0);

	RunProgramProtocol protocol(*mCommunicator, configVersion);

	signalsTester.expectSignal(&protocol, &RunProgramProtocol::error, "error");

	protocol.run(QFileInfo("test"));

	signalsTester.wait(250);

	ASSERT_TRUE(signalsTester.allIsGood());
}
