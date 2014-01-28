#include "interpreterTest.h"

#include <src/interpreter/interpreter.h>

#include <interpreterBase/robotModel/configurationInterfaceMock.h>

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	DummyBlockFactory *blocksFactory = new DummyBlockFactory();

	mQrguiFacade.reset(new QrguiFacade("unittests/testModel.qrs"));

	ConfigurationInterfaceMock configurationInterfaceMock;

	ON_CALL(mModel, needsConnection()).WillByDefault(Return(false));

	ON_CALL(mModel, init()).WillByDefault(Invoke([&] {
		QMetaObject::invokeMethod(&mModel, "connected", Q_ARG(bool, true));
		QMetaObject::invokeMethod(&mModel.configuration(), "allDevicesConfigured");
	}));

	ON_CALL(mModel, configuration()).WillByDefault(ReturnRef(configurationInterfaceMock));

	EXPECT_CALL(mModel, configuration()).Times(AtLeast(1));
	EXPECT_CALL(mModel, needsConnection()).Times(AtLeast(1));
	EXPECT_CALL(mModel, init()).Times(AtLeast(1));

	QAction fakeConnectToRobotAction(nullptr);

	mInterpreter.reset(new Interpreter(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->projectManagementInterface()
			, blocksFactory
			, &mModel
			, fakeConnectToRobotAction
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
