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

/// Test case for Qt Script TRIK generator.
class TrikV62QtsGeneratorTest : public testing::Test
{
protected:
	void SetUp() override;

	/// Provides access to kit plugin configurer object for tests.
	kitBase::KitPluginConfigurator &kitPluginConfigurer();

	/// Provides access to network robot simulator simulating control connection.
	tcpRobotSimulator::TcpRobotSimulator &controlSimulator();

private:
	/// Network server that simulates control connection to robot (on which all commands to robot are sent).
	QScopedPointer<tcpRobotSimulator::TcpRobotSimulator> mControlConnectionSimulator;

	/// Network server that simulates telemetry connection. It is actually a control connection server, so it sends
	/// nothing, but it is ok for generator tests.
	QScopedPointer<tcpRobotSimulator::TcpRobotSimulator> mTelemetryConnectionSimulator;

	/// RAII-style holder of registry settings used by tests. Restores registry to previous values when gets deleted.
	QScopedPointer<TestRegistry> mTestRegistry;

	/// qrgui facade. Emulates GUI (and most of QReal kernel functionality).
	QScopedPointer<QrguiFacade> mFacade;

	/// Plugin configurer object, needed to provide access to simulated kernel of QReal to a robot plugin.
	QScopedPointer<qReal::PluginConfigurator> mPluginConfigurer;

	/// Kit plugin configurer providing access to simulated robots plugin core functionality.
	QScopedPointer<kitBase::KitPluginConfigurator> mKitPluginConfigurer;

	/// Actual instance of Lua parser and interpreter, required by generator.
	QScopedPointer<qrtext::lua::LuaToolbox> mLuaToolbox;

	/// Simulated robot model manager, provides info about robot model to generator.
	QScopedPointer<kitBase::robotModel::RobotModelManagerInterface> mRobotModelManagerInterface;

	/// Interface with events from robots plugin core to kit plugin. Does nothing actually, since we do not have
	/// robots plugin core here.
	QScopedPointer<kitBase::EventsForKitPluginInterface> mEventsForKitPluginInterface;

	/// Interface that is used by generator to control interpreter. Since we do not have interpreter here, does
	/// nothing.
	QScopedPointer<kitBase::InterpreterControlInterface> mInterpreterControlInterface;

	/// Robot model that is used by robot model manager and is needed by generator for some reason despite it
	/// instantiates its own robot model.
	QScopedPointer<TestRobotModel> mRobotModel;
};

}
}
}
