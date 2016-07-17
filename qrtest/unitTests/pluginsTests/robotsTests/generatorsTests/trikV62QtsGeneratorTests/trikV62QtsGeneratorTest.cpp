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

#include "trikV62QtsGeneratorTest.h"

#include <trikV62QtsGeneratorPlugin.h>

#include <gmock/gmock.h>

#include <qrtext/lua/luaToolbox.h>
#include <testUtils/qrguiFacade.h>
#include <testUtils/wait.h>
#include <testUtils/testRegistry.h>
#include <kitBase/robotModel/commonRobotModel.h>
#include <qrkernel/settingsManager.h>

#include <mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelManagerInterfaceMock.h>
#include <mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/interpreterControlInterfaceMock.h>
#include <mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterMock.h>
#include <pluginsTests/robotsTests/support/testRobotModel.h>
#include <tcpRobotSimulator/tcpRobotSimulator.h>

using namespace qrTest;
using namespace qrTest::robotsTests::trikV62QtsGeneratorTests;
using namespace trik::qts;
using namespace ::testing;

void TrikV62QtsGeneratorTest::SetUp()
{
	mControlConnectionSimulator.reset(new tcpRobotSimulator::TcpRobotSimulator(8888));
	mTelemetryConnectionSimulator.reset(new tcpRobotSimulator::TcpRobotSimulator(9000));

	mControlConnectionSimulator->setConfigVersion("model-2015");

	mLuaToolbox.reset(new qrtext::lua::LuaToolbox());
	const auto robotModelManagerInterfaceMock = new RobotModelManagerInterfaceMock();
	mRobotModelManagerInterface.reset(robotModelManagerInterfaceMock);
	mEventsForKitPluginInterface.reset(new kitBase::EventsForKitPluginInterface());
	mInterpreterControlInterface.reset(new InterpreterControlInterfaceMock());

	mFacade.reset(new QrguiFacade("unittests/smile.qrs"));
	mPluginConfigurer.reset(new qReal::PluginConfigurator(
			mFacade->repoControlInterface()
			, mFacade->graphicalModelAssistInterface()
			, mFacade->logicalModelAssistInterface()
			, mFacade->controller()
			, mFacade->mainWindowInterpretersInterface()
			, mFacade->mainWindowDockInterface()
			, mFacade->projectManagementInterface()
			, mFacade->sceneCustomizer()
			, mFacade->systemEvents()
			, mFacade->textManager()
			));

	mKitPluginConfigurer.reset(new kitBase::KitPluginConfigurator(
			*mPluginConfigurer
			, *mRobotModelManagerInterface
			, *mLuaToolbox
			, *mEventsForKitPluginInterface
			, *mInterpreterControlInterface
			));

	mTestRegistry.reset(new TestRegistry);
	mTestRegistry->set("pathToGeneratorRoot", ".");
	mTestRegistry->set("TrikTcpServer", "127.0.0.1");

	mRobotModel.reset(new TestRobotModel());

	ON_CALL(*robotModelManagerInterfaceMock, model()).WillByDefault(Invoke(
			[this]() -> kitBase::robotModel::RobotModelInterface & {
				return *mRobotModel;
			}
			));

	EXPECT_CALL(*robotModelManagerInterfaceMock, model()).Times(AtLeast(1));
}

kitBase::KitPluginConfigurator &TrikV62QtsGeneratorTest::kitPluginConfigurer()
{
	return *mKitPluginConfigurer;
}

tcpRobotSimulator::TcpRobotSimulator &TrikV62QtsGeneratorTest::controlSimulator()
{
	return *mControlConnectionSimulator;
}

TEST_F(TrikV62QtsGeneratorTest, runProgramTest)
{
	TrikV62QtsGeneratorPlugin plugin;
	plugin.init(kitPluginConfigurer());

	const QList<qReal::ActionInfo> actions = plugin.customActions();
	qReal::ActionInfo runProgramAction = actions.at(2);
	runProgramAction.action()->trigger();

	Wait waiter(5000);
	waiter.stopAt(&controlSimulator(), &tcpRobotSimulator::TcpRobotSimulator::runProgramRequestReceivedSignal);
	waiter.wait();

	EXPECT_TRUE(controlSimulator().configVersionRequestReceived());
	EXPECT_TRUE(controlSimulator().runProgramRequestReceived());
}

TEST_F(TrikV62QtsGeneratorTest, incorrectCasingVersionTest)
{
	controlSimulator().setConfigVersion("model-2014");

	const qReal::ErrorReporterInterface *errorReporter
			= kitPluginConfigurer().qRealConfigurator().mainWindowInterpretersInterface().errorReporter();

	const ErrorReporterMock *errorReporterMock = static_cast<const ErrorReporterMock *>(errorReporter);

	TrikV62QtsGeneratorPlugin plugin;
	plugin.init(kitPluginConfigurer());

	const QList<qReal::ActionInfo> actions = plugin.customActions();
	qReal::ActionInfo runProgramAction = actions.at(2);
	runProgramAction.action()->trigger();

	Wait waiter(1000);
	waiter.stopAt(errorReporterMock, &ErrorReporterMock::error);
	waiter.wait();

	EXPECT_TRUE(controlSimulator().configVersionRequestReceived());
	EXPECT_FALSE(controlSimulator().runProgramRequestReceived());
	EXPECT_TRUE(errorReporter->wereErrors());
}
