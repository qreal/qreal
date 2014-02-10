#include "interpreterTest.h"

#include <src/interpreter/interpreter.h>

#include <interpreterBase/robotModel/configurationInterfaceMock.h>

#include <src/textLanguage/robotsBlockParser.h>

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	mQrguiFacade.reset(new QrguiFacade("unittests/testModel.qrs"));

	DummyBlockFactory *blocksFactory = new DummyBlockFactory();

	mFakeConnectToRobotAction.reset(new QAction(nullptr));

	ConfigurationInterfaceMock configurationInterfaceMock;

	ON_CALL(mModel, needsConnection()).WillByDefault(Return(false));
	EXPECT_CALL(mModel, needsConnection()).Times(AtLeast(1));

	ON_CALL(mModel, init()).WillByDefault(Invoke([&] {
		QMetaObject::invokeMethod(&mModel, "connected", Q_ARG(bool, true));
		QMetaObject::invokeMethod(&mModel.mutableConfiguration(), "allDevicesConfigured");
	}));
	EXPECT_CALL(mModel, init()).Times(AtLeast(1));

	ON_CALL(mModel, configuration()).WillByDefault(ReturnRef(configurationInterfaceMock));
	EXPECT_CALL(mModel, configuration()).Times(AtLeast(1));

	ON_CALL(mModelManager, model()).WillByDefault(ReturnRef(mModel));
	EXPECT_CALL(mModelManager, model()).Times(AtLeast(1));

	ON_CALL(mModel, mutableConfiguration()).WillByDefault(ReturnRef(configurationInterfaceMock));
	EXPECT_CALL(mModel, mutableConfiguration()).Times(AtLeast(1));

	ON_CALL(mBlocksFactoryManager, addFactory(_)).WillByDefault(Return());
	EXPECT_CALL(mBlocksFactoryManager, addFactory(_)).Times(0);

	ON_CALL(mBlocksFactoryManager, block(_)).WillByDefault(
			Invoke([&] (qReal::Id const &id) { return blocksFactory->block(id); } )
			);
	EXPECT_CALL(mBlocksFactoryManager, block(_)).Times(AtLeast(0));

	ON_CALL(mBlocksFactoryManager, providedBlocks()).WillByDefault(
			Invoke([&] { return blocksFactory->providedBlocks(); } )
			);
	EXPECT_CALL(mBlocksFactoryManager, providedBlocks()).Times(0);

//	ON_CALL(configurationInterfaceMock, lock)

	/// @todo Don't like it.
	interpreterCore::textLanguage::RobotsBlockParser parser(
			mQrguiFacade->mainWindowInterpretersInterface().errorReporter()
			);

	mInterpreter.reset(new Interpreter(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->projectManagementInterface()
			, mBlocksFactoryManager
			, mModelManager
			, parser
			, *mFakeConnectToRobotAction
			));
}

TEST_F(InterpreterTest, interpret)
{
	mInterpreter->interpret();
}

TEST_F(InterpreterTest, stopRobot)
{
	EXPECT_CALL(mModel, stopRobot()).Times(1);

	mInterpreter->interpret();
	mInterpreter->stopRobot();
}
