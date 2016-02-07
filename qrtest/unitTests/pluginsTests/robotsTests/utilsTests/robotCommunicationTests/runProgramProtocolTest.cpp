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

#include <utils/robotCommunication/runProgramProtocol.h>

#include <QtCore/QFileInfo>

#include <gtest/gtest.h>

#include <utils/signalsTester.h>
#include <utils/delay.h>

#include "support/tcpRobotCommunicatorMock.h"

using namespace utils::robotCommunication;
using namespace qrTest;
using namespace ::testing;

TEST(runProgramProtocolTest, mainExecutionTest)
{
	SignalsTester signalsTester{};

	const QString configVersion = "model-2015";
	TcpRobotCommunicatorMock communicator;

	ON_CALL(communicator, requestCasingVersion()).WillByDefault(
		Invoke([&communicator]() {
			delay([&communicator](){ emit communicator.casingVersionReceived("model-2015"); });
		})
	);

	ON_CALL(communicator, uploadProgram(_)).WillByDefault(
		Invoke([&communicator](const QString &name) {
			Q_UNUSED(name)
			delay([&communicator]() { emit communicator.uploadProgramDone(); });
		})
	);

	ON_CALL(communicator, runProgram(_)).WillByDefault(
		Invoke([&communicator](const QString &name) {
			Q_UNUSED(name)
			delay([&communicator]() { emit communicator.startedRunning(); });
		})
	);

	EXPECT_CALL(communicator, requestCasingVersion()).Times(1);
	EXPECT_CALL(communicator, uploadProgram(_)).Times(1);
	EXPECT_CALL(communicator, runProgram(_)).Times(1);

	RunProgramProtocol protocol(communicator, configVersion);

	signalsTester.expectSignal(&protocol, &RunProgramProtocol::success, "success");

	protocol.run(QFileInfo("test"));

	signalsTester.wait(1);

	ASSERT_TRUE(signalsTester.allIsGood());
}
