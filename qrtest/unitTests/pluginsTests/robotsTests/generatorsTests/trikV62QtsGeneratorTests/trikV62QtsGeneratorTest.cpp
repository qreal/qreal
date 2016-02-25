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
#include <kitBase/robotModel/commonRobotModel.h>
#include <qrkernel/settingsManager.h>

#include <mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelManagerInterfaceMock.h>
#include <mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/interpreterControlInterfaceMock.h>
#include <pluginsTests/robotsTests/support/testRobotModel.h>
#include <tcpRobotSimulator/tcpRobotSimulator.h>

using namespace qrTest;
using namespace qrTest::robotsTests::trikV62QtsGeneratorTests;
using namespace trik::qts;
using namespace ::testing;

void TrikV62QtsGeneratorTest::SetUp()
{
}

TEST_F(TrikV62QtsGeneratorTest, runProgramTest)
{
	tcpRobotSimulator::TcpRobotSimulator simulator;

	qReal::SettingsManager::setValue("pathToGeneratorRoot", ".");
	qReal::SettingsManager::setValue("TrikTcpServer", "127.0.0.1");

	QrguiFacade facade("unittests/smile.qrs");

	qReal::PluginConfigurator configurer(
			facade.repoControlInterface()
			, facade.graphicalModelAssistInterface()
			, facade.logicalModelAssistInterface()
			, facade.mainWindowInterpretersInterface()
			, facade.mainWindowDockInterface()
			, facade.projectManagementInterface()
			, facade.sceneCustomizer()
			, facade.systemEvents()
			, facade.textManager()
			);

	qrtext::lua::LuaToolbox luaToolbox;
	RobotModelManagerInterfaceMock robotModelManagerInterfaceMock;
	kitBase::EventsForKitPluginInterface eventsForKitPluginInterface;
	InterpreterControlInterfaceMock interpreterControlInterfaceMock;

	TestRobotModel robotModel;

	ON_CALL(robotModelManagerInterfaceMock, model()).WillByDefault(Invoke(
			[&robotModel]() -> kitBase::robotModel::RobotModelInterface & {
				return robotModel;
			}
			));

	kitBase::KitPluginConfigurator kitPluginConfigurer(
			configurer
			, robotModelManagerInterfaceMock
			, luaToolbox
			, eventsForKitPluginInterface
			, interpreterControlInterfaceMock
			);

	TrikV62QtsGeneratorPlugin plugin;
	plugin.init(kitPluginConfigurer);

	const QList<qReal::ActionInfo> actions = plugin.customActions();
	qReal::ActionInfo runProgramAction = actions.at(2);
	runProgramAction.action()->trigger();

	Wait::wait(5000);

	EXPECT_TRUE(simulator.configVersionRequestReceived());
	EXPECT_TRUE(simulator.runProgramRequestReceived());
}
