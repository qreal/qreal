#include "interpreterTest.h"

#include <QtCore/QCoreApplication>

#include <qrtext/lua/luaToolbox.h>
#include "src/interpreter/interpreter.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	/// @todo: Need to rewrite this shit with 'temp' setting manager value.
	/// It is used in serializer and innitialized in main window.
	/// So when we run tests outside of enviroment they fail!
	qReal::SettingsManager::setValue("temp", QCoreApplication::applicationDirPath() + "/unsaved");
	mQrguiFacade.reset(new QrguiFacade("unittests/basicTest.qrs"));
	mQrguiFacade->setActiveTab(qReal::Id::loadFromString(
			"qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{f08fa823-e187-4755-87ba-e4269ae4e798}"));

	mFakeConnectToRobotAction.reset(new QAction(nullptr));

	ON_CALL(mConfigurationInterfaceMock, devices()).WillByDefault(
			Return(QList<interpreterBase::robotModel::robotParts::Device *>())
			);
	EXPECT_CALL(mConfigurationInterfaceMock, devices()).Times(AtLeast(1));

	/// @todo: Do we need this code in some common place? Why do we need to write
	/// it every time when we are going to use RobotModelManager mock?

	ON_CALL(mModel, name()).WillByDefault(Return("mockRobot"));
	EXPECT_CALL(mModel, name()).Times(AtLeast(1));

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

	ON_CALL(mModel, configurablePorts()).WillByDefault(Return(QList<interpreterBase::robotModel::PortInfo>()));
	EXPECT_CALL(mModel, configurablePorts()).Times(AtLeast(0));

	ON_CALL(mModel, availablePorts()).WillByDefault(Return(QList<interpreterBase::robotModel::PortInfo>()));
	EXPECT_CALL(mModel, availablePorts()).Times(AtLeast(0));

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
			Invoke([=] (qReal::Id const &id, interpreterBase::robotModel::RobotModelInterface const &robotModel) {
					Q_UNUSED(robotModel)
					return blocksFactory->block(id);
			} )
			);
	EXPECT_CALL(mBlocksFactoryManager, block(_, _)).Times(AtLeast(0));

	ON_CALL(mBlocksFactoryManager, enabledBlocks(_)).WillByDefault(
			Invoke([=] (interpreterBase::robotModel::RobotModelInterface const &robotModel) {
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
}

TEST_F(InterpreterTest, interpret)
{
	EXPECT_CALL(mModel, stopRobot()).Times(1);

	mInterpreter->interpret();
}

TEST_F(InterpreterTest, stopRobot)
{
	// It shall be called directly here and in destructor of a model.
	EXPECT_CALL(mModel, stopRobot()).Times(2);

	mInterpreter->interpret();
	mInterpreter->stopRobot();
}
