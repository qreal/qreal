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

#include "interpreterTest.h"

#include <QtCore/QCoreApplication>

#include <qrtext/lua/luaToolbox.h>
#include "interpreterCore/interpreter/interpreter.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	mQrguiFacade.reset(new QrguiFacade("unittests/basicTest.qrs"));
	mQrguiFacade->setActiveTab(qReal::Id::loadFromString(
			"qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{f08fa823-e187-4755-87ba-e4269ae4e798}"));

	mFakeConnectToRobotAction.reset(new QAction(nullptr));

	ON_CALL(mConfigurationInterfaceMock, devices()).WillByDefault(
			Return(QList<kitBase::robotModel::robotParts::Device *>())
			);
	EXPECT_CALL(mConfigurationInterfaceMock, devices()).Times(AtLeast(1));

	/// @todo: Do we need this code in some common place? Why do we need to write
	/// it every time when we are going to use RobotModelManager mock?

	ON_CALL(mModel, robotId()).WillByDefault(Return("mockRobot"));
	EXPECT_CALL(mModel, robotId()).Times(AtLeast(1));

	ON_CALL(mModel, name()).WillByDefault(Return("mockRobot"));

	ON_CALL(mModel, needsConnection()).WillByDefault(Return(false));
	EXPECT_CALL(mModel, needsConnection()).Times(AtLeast(0));

	ON_CALL(mModel, init()).WillByDefault(Return());
	EXPECT_CALL(mModel, init()).Times(AtLeast(0));

	ON_CALL(mModel, configuration()).WillByDefault(ReturnRef(mConfigurationInterfaceMock));
	EXPECT_CALL(mModel, configuration()).Times(AtLeast(0));

	ON_CALL(mModel, connectToRobot()).WillByDefault(
			Invoke(&mModelManager, &RobotModelManagerInterfaceMock::emitConnected)
			);
	EXPECT_CALL(mModel, connectToRobot()).Times(AtLeast(0));

	ON_CALL(mModel, disconnectFromRobot()).WillByDefault(
			Invoke(&mModelManager, &RobotModelManagerInterfaceMock::emitDisconnected)
			);
	EXPECT_CALL(mModel, disconnectFromRobot()).Times(AtLeast(0));

	ON_CALL(mModel, configurablePorts()).WillByDefault(Return(QList<kitBase::robotModel::PortInfo>()));
	EXPECT_CALL(mModel, configurablePorts()).Times(AtLeast(0));

	ON_CALL(mModel, availablePorts()).WillByDefault(Return(QList<kitBase::robotModel::PortInfo>()));
	EXPECT_CALL(mModel, availablePorts()).Times(AtLeast(0));

	ON_CALL(mModel, buttonCodes()).WillByDefault(Return(StringIntHash()));
	EXPECT_CALL(mModel, buttonCodes()).Times(AtLeast(1));

	ON_CALL(mModel, applyConfiguration()).WillByDefault(
			Invoke(&mModelManager, &RobotModelManagerInterfaceMock::emitAllDevicesConfigured)
			);
	EXPECT_CALL(mModel, applyConfiguration()).Times(1);

	ON_CALL(mModel, connectionState()).WillByDefault(Return(RobotModelInterfaceMock::connectedState));
	EXPECT_CALL(mModel, connectionState()).Times(2);

	ON_CALL(mModel, timeline()).WillByDefault(ReturnRef(mTimeline));
	EXPECT_CALL(mModel, timeline()).Times(AtLeast(1));


	ON_CALL(mModelManager, model()).WillByDefault(ReturnRef(mModel));
	EXPECT_CALL(mModelManager, model()).Times(AtLeast(1));

	ON_CALL(mBlocksFactoryManager, addFactory(_, _)).WillByDefault(Return());
	EXPECT_CALL(mBlocksFactoryManager, addFactory(_, _)).Times(0);

	mParser.reset(new interpreterCore::textLanguage::RobotsBlockParser(mModelManager, []() { return 0; }));

	DummyBlockFactory *blocksFactory = new DummyBlockFactory;
	blocksFactory->configure(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mModelManager
			, *mQrguiFacade->mainWindowInterpretersInterface().errorReporter()
			, *mParser
			);

	ON_CALL(mBlocksFactoryManager, block(_, _)).WillByDefault(
			Invoke([=] (qReal::Id const &id, kitBase::robotModel::RobotModelInterface const &robotModel) {
					Q_UNUSED(robotModel)
					return blocksFactory->block(id);
			} )
			);
	EXPECT_CALL(mBlocksFactoryManager, block(_, _)).Times(AtLeast(0));

	ON_CALL(mBlocksFactoryManager, enabledBlocks(_)).WillByDefault(
			Invoke([=] (kitBase::robotModel::RobotModelInterface const &robotModel) {
					Q_UNUSED(robotModel)
					return blocksFactory->providedBlocks().toSet();
			} )
			);
	EXPECT_CALL(mBlocksFactoryManager, enabledBlocks(_)).Times(0);

	mInterpreter.reset(new Interpreter(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->projectManagementInterface()
			, mBlocksFactoryManager
			, mModelManager
			, *mParser
			, *mFakeConnectToRobotAction
			));

	mInterpreterStopped = false;
	QObject::connect(mInterpreter.data(), &InterpreterInterface::stopped, [this]() {
		mEventLoop.exit();
		mInterpreterStopped = true;
	});
}

TEST_F(InterpreterTest, interpret)
{
	EXPECT_CALL(mModel, stopRobot()).Times(2);

	mInterpreter->interpret();
	if (!mInterpreterStopped) {
		// Queued connections must work!
		mEventLoop.exec();
	}
}

TEST_F(InterpreterTest, stopRobot)
{
	// It shall be called directly here, before interpretation and in destructor of a model.
	EXPECT_CALL(mModel, stopRobot()).Times(3);

	mInterpreter->interpret();
	if (!mInterpreterStopped) {
		// Queued connections must work!
		mEventLoop.exec();
	}

	mInterpreter->stopRobot(qReal::interpretation::StopReason::finised);
}
