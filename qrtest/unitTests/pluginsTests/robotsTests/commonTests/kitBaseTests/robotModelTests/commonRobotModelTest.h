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

#include <QtCore/QTimer>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <kitBase/robotModel/commonRobotModel.h>
#include <support/dummyDevice.h>

namespace qrTest {
namespace robotsTests {
namespace kitBaseTests {

/// Tests for CommonRobotModel.
class CommonRobotModelTest : public testing::Test
{
public:

	/// Partially mocked robot model.
	class CommonRobotModelDescendantMock : public kitBase::robotModel::CommonRobotModel
	{
	public:
		/// Constructor.
		/// @param immediateConnection - if true, model connects to robot without delay, as 2d model, if false --- after
		///        some timeout.
		explicit CommonRobotModelDescendantMock(bool immediateConnection = false);

		MOCK_CONST_METHOD0(name, QString());
		MOCK_CONST_METHOD0(friendlyName, QString());
		MOCK_CONST_METHOD0(needsConnection, bool());
		MOCK_CONST_METHOD0(priority, int());

		void connectToRobot() override;

		MOCK_CONST_METHOD0(convertibleBases, QList<kitBase::robotModel::DeviceInfo>());

	private:
		kitBase::robotModel::robotParts::Device * createDevice(
				kitBase::robotModel::PortInfo const &port
				, kitBase::robotModel::DeviceInfo const &deviceInfo
				) override;

		QTimer mConnectionTimer;
		bool mImmediateConnection;
	};
};

}
}
}
