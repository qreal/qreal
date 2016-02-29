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

#pragma once

#include <gtest/gtest.h>

#include <QtCore/QScopedPointer>

namespace qrtext {
namespace lua {
class LuaToolbox;
}
}

namespace tcpRobotSimulator {
class TcpRobotSimulator;
}

namespace qReal {
class PluginConfigurator;
}

namespace kitBase {
class EventsForKitPluginInterface;
class InterpreterControlInterface;
class KitPluginConfigurator;

namespace robotModel {
class RobotModelManagerInterface;
}

}

namespace qrTest {

class QrguiFacade;
class TestRegistry;
class TestRobotModel;

namespace robotsTests {
namespace trikV62QtsGeneratorTests {

class TrikV62QtsGeneratorTest : public testing::Test
{
protected:
	void SetUp() override;

	kitBase::KitPluginConfigurator &kitPluginConfigurer();
	tcpRobotSimulator::TcpRobotSimulator &controlSimulator();

private:
	QScopedPointer<tcpRobotSimulator::TcpRobotSimulator> mControlConnectionSimulator;
	QScopedPointer<tcpRobotSimulator::TcpRobotSimulator> mTelemetryConnectionSimulator;
	QScopedPointer<TestRegistry> mTestRegistry;
	QScopedPointer<QrguiFacade> mFacade;
	QScopedPointer<qReal::PluginConfigurator> mPluginConfigurer;
	QScopedPointer<kitBase::KitPluginConfigurator> mKitPluginConfigurer;

	QScopedPointer<qrtext::lua::LuaToolbox> mLuaToolbox;
	QScopedPointer<kitBase::robotModel::RobotModelManagerInterface> mRobotModelManagerInterface;
	QScopedPointer<kitBase::EventsForKitPluginInterface> mEventsForKitPluginInterface;
	QScopedPointer<kitBase::InterpreterControlInterface> mInterpreterControlInterface;

	QScopedPointer<TestRobotModel> mRobotModel;
};

}
}
}
